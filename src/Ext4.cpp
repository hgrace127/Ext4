#include <iostream>
#include <List>

#include "Ext4.hpp"
#include "Ext4_Extent.hpp"
#include "Extent.hpp"
#include "DirectoryEntry.hpp"
#include "byte_buffer2.hpp"

using namespace std;

Ext4::Ext4(fstream* stream, long startAddress = 0L, bool isLive = false)
{
    m_startAddress = startAddress;
    m_indirectExtents = new vector<Extent*>();
    try
    {
        m_stream = stream;
        m_isLive = isLive;
        m_isValid = initExt4();
    }
    catch (const exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


auto Ext4::initExt4() -> bool
{
    int bst[] = { 0x400, 0x800, 0x1000 };

    try
    {
        if (m_stream->is_open()){
            uint8_t buffer[1024];
            m_stream->seekg(0x400);
            m_stream->read((char*)buffer, 0x400);

            m_superblock = new Superblock(buffer, 0);
        }

        if (!m_superblock->IsValid())
            return false;

        m_blkGroupDescTable = new list<BlockGroupDescriptor*>();
        m_blockSize = bst[m_superblock->logBlkSize];
        m_iNodePerBlock = m_blockSize / m_superblock->iNodeSize;
        m_iNodeBlockCount = (int)m_superblock->iNodePerGroup / m_iNodePerBlock;
        m_blockGroupCount = (int)((m_superblock->blkCnt + m_superblock->blkPerGroup - 1) / m_superblock->blkPerGroup);
        
        m_size = m_superblock->blkCnt * m_blockSize;

        makeBlkGroupDescriptorTable();

        m_rootNode = makeRootNode();

    }
    catch (const exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return true;
}

auto Ext4::makeBlkGroupDescriptorTable() -> void
{
    long startAddr = m_blockSize;
    long limit = 0;
    
    uint8_t* b0 = new uint8_t[m_superblock->blkPerGroup];
    auto blkGrpSize = m_superblock->BlkGrouDescSize();

    if (startAddr == 0x400)
        startAddr += 0x400;

    for(int i = 0; i < m_blockGroupCount; i++){
        long offset = i * blkGrpSize;
        if (offset >= limit){
            m_stream->read((char*)b0, m_superblock->blkPerGroup);
            limit += (int)m_superblock->blkPerGroup;
        }

        BlockGroupDescriptor* blkGroup = new BlockGroupDescriptor(b0, offset % (int)m_superblock->blkPerGroup);
        if (blkGroup->IsEmpty())
            break;
        blkGroup->startAddrOfBlk = (unsigned int) (i * m_superblock->blkPerGroup);
        m_blkGroupDescTable->push_back(blkGroup);
    }

    delete b0;   
}

auto Ext4::makeRootNode() -> Node*
{
    DirectoryEntry* de = new DirectoryEntry(2);
    de->SetName("/");

    Node* node = makeNode(de);
    //return node->IsDirectory() ? node : makeEmptyRoot();
    return nullptr;
}

auto Ext4::makeNode(DirectoryEntry* de) -> Node*
{
    INode* iNode = findiNode(de->m_iNodeNo);
    
    Node* node = makeNode(iNode, de->m_name, true);

    node->m_iNodeNo = de->m_iNodeNo;

    return node;
}


auto Ext4::makeNode(INode* inode, string name = "", bool active = true) -> Node*
{
    auto type = NodeType::None;

    switch (inode->m_fileMode & 0xF000)
    {
        case 0x4000: type = NodeType::Directory; break;
        case 0x8000: type = NodeType::File;      break;
        case 0xA000: type = NodeType::SoftLink;  break;
    }

    if (name.empty())
        name = to_string(inode->m_address);

    // if (type == NodeType::SoftLink)
    //     return new Node(name, type, nullptr);

    // Node* n0 = new Node(name, type, )

    return nullptr;
}

auto Ext4::findiNode(uint32_t no) -> INode*
{  
    return nullptr;
}

auto Ext4::nodeStreamFrom(INode inode, uint8_t* extentsBuffer, bool active=true) -> NodeStream*
{
    long fileSize = inode.m_fileSize;
    if (fileSize <= 0)
        return new NodeStream();

    vector<Extent*>* extents = new vector<Extent*>();

    long expectedLogicalBlkNo = 0L;
    vector<Ext4_Extent*>* extExtents = buildExtentsFrom(inode, extentsBuffer, &expectedLogicalBlkNo, active);

    if(extExtents != nullptr && extExtents->size() > 0)
    {
        int64_t count = m_blockSize * ((fileSize + m_blockSize - 1) / m_blockSize);

        Ext4_Extent* ez = extExtents->at(extExtents->size() -1);
        if(count > m_blockSize * ez->m_LOffset + ez->m_count)
        {
            long endingZeroSize = (count - (m_blockSize * ez->m_LOffset + ez->m_count));
            Ext4_Extent* ext = new Ext4_Extent(0, -1L, endingZeroSize);
            extExtents->push_back(ext);
        }
        for (int i=0; i< extExtents->size(); i++)
        {
            extents->push_back(new Extent(m_stream, extExtents->at(i)->m_LOffset, extExtents->at(i)->m_POffset));    
        }
    }

    return new NodeStream(extents, false ,fileSize);
}



auto Ext4::buildExtentsFrom(INode inode, uint8_t* extentsBuffer, long* expectedLogicalBlkNo, bool active=true) -> vector<Ext4_Extent*>*
{
    vector<Extent>* extExtents = new vector<Extent>();
    
    ByteBuffer2 bb(extentsBuffer, 0, m_iNodeSize);
    uint16_t signature = bb.get_uint16_le();

    uint16_t extentCount = bb.get_uint16_le(2);
    uint16_t depth = bb.get_uint16_le(6);

    if (depth > 0)
    {
        for (int i = 1; i <= extentCount; i++)
        {
            int offset = i * 12;
            uint32_t logicalBlkNo = bb.get_uint32_le();
            uint64_t startBlkNo = bb.get_int64_le();
            startBlkNo &= 0xFFFFFFFFFFFF;

            if (active)
                m_indirectExtents->push_back(new Extent(m_stream, startBlkNo * m_blockSize, m_blockSize));

            uint8_t* indirect = new uint8_t[m_blockSize];

            m_stream->seekg(startBlkNo * m_blockSize);
            m_stream->read((char*)indirect, m_blockSize);

            vector<Ext4_Extent*>* extents = buildExtentsFrom(inode, indirect, expectedLogicalBlkNo);

            if (extents == nullptr)
                return nullptr;

            vector<Ext4_Extent*>::iterator it;
            // for(it = extExtents->begin(); it != extExtents->end(); it++){

            // }


        }
    } 
    return nullptr;
}