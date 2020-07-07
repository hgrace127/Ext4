#include <iostream>
#include <vector>

#include "Ext4.hpp"
#include "Extent.hpp"
#include "NodeStream.hpp"
#include "DirectoryEntry.hpp"
#include "byte_buffer2.hpp"

using namespace std;

Ext4::Ext4(ifstream* stream, long startAddress = 0L, bool isLive = false)
{
    m_start_address = startAddress;

    try
    {
        m_stream   = stream;
        m_is_live  = isLive;
        m_is_valid = init_ext4();
    }
    catch (const exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


auto Ext4::init_ext4() -> bool
{
    int bst[] = { 0x400, 0x800, 0x1000 };

    try
    {
        if (m_stream->is_open())
        {
            uint8_t buffer[0x400];
            m_stream->seekg(0x400);
            m_stream->read((char*)buffer, 0x400);

            m_superblock = new Superblock(buffer, 0);
        }

        if (!m_superblock->is_valid())
            return false;

        m_block_size = bst[m_superblock->m_log_blk_size];
        m_inode_per_block = m_block_size / m_superblock->m_inode_size;
        m_inode_block_count = (int)m_superblock->m_inode_per_group / m_inode_per_block;
        m_block_group_count = (int)((m_superblock->m_blk_cnt + m_superblock->m_blk_per_group - 1) / m_superblock->m_blk_per_group);
        
        m_inode_size = m_superblock->m_inode_size;
        m_size = m_superblock->m_blk_cnt * m_block_size;
        m_blk_group_desc_table = make_blk_group_descriptor_table();
        m_root_node = make_root_node();
    }
    catch (const exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return true;
}

auto Ext4::make_blk_group_descriptor_table() -> std::vector<BlockGroupDescriptor>
{
    long start_addr = m_block_size;
    long limit = 0;
    
    auto blk_group_size = m_superblock->blk_group_desc_size();

    if (start_addr == 0x400)
        start_addr += 0x400;

    std::vector<BlockGroupDescriptor> blk_group_desc_tables;

    for (int i = 0; i < m_block_group_count; i++)
    {
        uint8_t* b0 = new uint8_t[m_superblock->m_blk_per_group];
        long offset = i * blk_group_size;
        if (offset >= limit)
        {
            m_stream->read((char*)b0, m_superblock->m_blk_per_group);
            limit += (int)m_superblock->m_blk_per_group;
        }

        BlockGroupDescriptor blk_group(b0, offset % (int)m_superblock->m_blk_per_group);

        if (blk_group.is_empty())
            break;

        blk_group.start_addr_of_blk = (unsigned int) (i * m_superblock->m_blk_per_group);
        blk_group_desc_tables.push_back(blk_group);
    }

    return blk_group_desc_tables;
}

auto Ext4::make_root_node() -> Node*
{
    DirectoryEntry* de = new DirectoryEntry(2);
    de->set_name("/");
    Node* node = make_node(de);
    node->m_is_root = true;

    return node->is_directory() ? node : make_empty_root();
}

auto Ext4::make_node(DirectoryEntry* de) -> Node*
{
    INode* inode = find_inode(de->m_inode_no);
    Node* node = make_node(inode, de->m_name, true);

    node->m_iNode_no = de->m_inode_no;

    return node;
}


auto Ext4::make_node(INode* inode, string name = "", bool active = true) -> Node*
{

    auto type = NodeType::None;

    switch (inode->m_file_mode & 0xF000)
    {
        case 0x4000: type = NodeType::Directory; break;
        case 0x8000: type = NodeType::File;      break;
        case 0xA000: type = NodeType::SoftLink;  break;
    }

    if (name.empty())
        name = to_string(inode->m_address);

    if (type == NodeType::SoftLink)
        return new Node(name, type, nullptr, NodeAttr::Normal);

    Node* n0 = new Node(name, type, NodeState::Active, node_stream_from(inode, inode->m_blk_pointers, active), NodeAttr::Normal, 0L);
    
    n0->m_uid = inode->m_UID;
    n0->m_gid = inode->m_GID;
    n0->m_filemode = inode->m_file_mode;

    return n0;
}

auto Ext4::find_inode(uint32_t no) -> INode*
{  
    try
    {
        uint8_t b[0x1000];

        int blk_group_no = (int)((no - 1) / m_superblock->m_inode_per_group);
        int inode_index = (int)((no - 1) % m_superblock->m_inode_per_group);
        uint32_t blk_group = m_blk_group_desc_table[blk_group_no].start_blk_addr_of_inode_table;

        long byte_offset = blk_group * m_block_size + inode_index * m_superblock->m_inode_size;

        m_stream->seekg(byte_offset, m_stream->beg);
        m_stream->read((char*)b, 0x1000);

        return new INode(b, 0L, (long)m_superblock->m_inode_size, byte_offset);
    }
    catch(const exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}

auto Ext4::node_stream_from(INode *inode, uint8_t* extents_buffer, bool active=true) -> NodeStream*
{
    long file_size = inode->m_file_size;
    if (file_size <= 0)
        return new NodeStream();

    vector<Extent*> extents;

    long expected_logical_blk_no = 0L;
    vector<Ext4Extent*> ext4_extents = build_extents_from(inode, extents_buffer, &expected_logical_blk_no, active);

    if(!ext4_extents.empty() && ext4_extents.size() > 0)
    {
        int64_t count = m_block_size * ((file_size + m_block_size - 1) / m_block_size);

        Ext4Extent* ez = ext4_extents[ext4_extents.size() -1];
        if(count > m_block_size * ez->m_l_offset + ez->m_count)
        {
            long ending_zero_size = (count - (m_block_size * ez->m_l_offset + ez->m_count));
            Ext4Extent* ext = new Ext4Extent(0, -1L, ending_zero_size);
            ext4_extents.push_back(ext);
        }
        for (int i=0; i< ext4_extents.size(); i++)
        {
            extents.push_back(new Extent(m_stream, ext4_extents[i]->m_p_offset, ext4_extents[i]->m_count));
        }
    }

    return new NodeStream(extents, false , file_size);
}



auto Ext4::build_extents_from(INode *inode, uint8_t* extents_buffer, long* expected_logical_blk_no, bool active=true) -> vector<Ext4Extent*>
{
    vector<Ext4Extent*> ext_extents;
    
    ByteBuffer2 bb(extents_buffer, 0, m_inode_size + 28); // m_iNodeSize�� �ִ°� �³� ..?
    uint16_t signature = bb.get_uint16_le();

    uint16_t extent_count = bb.get_uint16_le(2);
    uint16_t depth = bb.get_uint16_le(6);

    if (depth > 0)
    {
        for (int i = 1; i <= extent_count; i++)
        {
            int offset = i * 12;
            uint32_t logical_blk_no = bb.get_uint32_le();
            uint64_t start_blk_no = bb.get_int64_le();
            start_blk_no &= 0xFFFFFFFFFFFF;

            if (active)
                m_indirect_extents.emplace_back(m_stream, start_blk_no * m_block_size, m_block_size);

            uint8_t* indirect = new uint8_t[m_block_size];

            m_stream->seekg(start_blk_no * m_block_size);
            m_stream->read((char*)indirect, m_block_size);

            vector<Ext4Extent*> extents = build_extents_from(inode, indirect, expected_logical_blk_no);

            if (extents.empty())
                return ext_extents;

            for(int i=0; i < extents.size(); i++){
                ext_extents.push_back(extents[i]);
            }
        }
    }
    else
    {
        for(int i = 1; i <= extent_count; i++)
        {
            long offset = i * 12;
            uint32_t logical_blk_no = bb.get_uint32_le(offset);

            if (logical_blk_no > (*expected_logical_blk_no))
            {
                long cnt1 = (logical_blk_no - (*expected_logical_blk_no)) * m_block_size;
                Ext4Extent* ext1 = new Ext4Extent(0, -1L, cnt1);
                ext_extents.push_back(ext1);
            }

            long no_of_blk = bb.get_uint16_le(offset + 4);
            if (no_of_blk > 0x8000)
                no_of_blk -= 0x8000;

            int64_t start_blk_no = bb.get_uint16_le(offset + 6);
            start_blk_no <<= 32;
            start_blk_no |= bb.get_uint32_le(offset + 8);

            *expected_logical_blk_no = no_of_blk + logical_blk_no;

            if (start_blk_no * m_block_size > m_size)
                return ext_extents; // ��ǻ� .size() == 0�� vector
            
            ext_extents.push_back(new Ext4Extent(logical_blk_no, start_blk_no * m_block_size, no_of_blk * m_block_size));
        }

        if (active && extent_count == 0)
            ext_extents.push_back(new Ext4Extent(0, -1L, 0));
    }
    
    return ext_extents;
}

auto Ext4::make_empty_root() -> Node*
{
    return new Node(NodeType::Directory);
}

auto Ext4::build_filesystem() -> void
{
    m_root_node->m_children->m_nodes.clear();

    expand_all(m_root_node);

    unfold_tree(m_root_node);
    
}

auto Ext4::expand_all(Node* node) -> bool
{
    if (node == nullptr)
        return false;

    int repeat = 10;

    for (int i = 0; i < repeat; i++)
    {
        try
        {
            node = expand(node, "");
            break;
        }
        catch (const exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    for each (Node* child in node->m_children->m_nodes)
    {
        if (child->is_expandable())
            if (!expand_all(child))
                return false;
    }

    return true;
}

auto Ext4::unfold_tree(Node* node) -> void
{

}

auto Ext4::expand(Node* from, string dir = "") -> Node*
{
    if (!from->is_directory())
        return nullptr;

    NodeStream* ns = from->m_stream;
    long len = ns->m_actual_length;

    if (len < 0)
        return nullptr;
    
    this->m_stream;

    // buffer 할당해제 필요할 수 있음.
    auto buffer = ns->read(m_stream, 0, len);
    long offset = 0;
    int nil_iteration = 0;

    while (offset < len)
    {
        DirectoryEntry* de = new DirectoryEntry(buffer, len, offset);

        try
        {
            if (de->is_soft_link())
                continue;

            if (dir != "" && dir != de->m_name)
                continue;

            if (de->is_empty() && de->has_valid_name())
            {

                Node* n0 = make_node(de);
                from->m_children->add(n0);
            }
            else
            {

            }
        }
        catch (const exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        offset += de->m_recode_length;
    }

    return from;
}