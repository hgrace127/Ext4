#include "iNode.hpp"
#include "byte_buffer2.hpp"

using namespace std;

INode::INode(uint8_t* b, long offset, long inodeSize, long imageOffset)
{
    ByteBuffer2 bb(b, offset, 0x1000, false); 

    m_address = imageOffset;
    m_hex_repr = "";

    m_file_mode    = bb.get_uint16_le(); // 0         
    m_UID         = bb.get_uint16_le(); // 2
    m_size        = bb.get_uint32_le(); // 4
    m_access_time  = bb.get_uint32_le(); // 8
    m_change_time  = bb.get_uint32_le(); // 12  NOTICE: inode change time
    m_modify_time  = bb.get_uint32_le(); // 16          file change time
    m_delete_time  = bb.get_uint32_le(); // 20
    m_GID         = bb.get_uint16_le(); // 24
    m_link_count   = bb.get_uint16_le(); // 26
    m_blk_count    = bb.get_uint32_le(); // 28
    m_flag        = bb.get_uint32_le(); // 32
    m_os_desc1     = bb.get_uint32_le(); // 36
    m_blk_pointers = bb.get_bytes(60);   // 40  NOTICE: in Ext3 4 * 15 = 60
    m_generation_number
                  = bb.get_uint32_le(); // 100
    m_file_ACL     = bb.get_uint32_le(); // 104
    m_dir_ACL      = bb.get_uint32_le(); // 108
    m_blk_addr_of_fragmentation
                  = bb.get_uint32_le(); // 112
    m_os_desc2     = bb.get_bytes(12);   // 116

    if (use_extents())
    {
        m_extrai_size = bb.get_uint16_le(); // extra fields
        m_chksum_Hi   = bb.get_uint16_le();
        m_ctime_extra = bb.get_uint32_le();
        m_mtime_extra = bb.get_uint32_le();
        m_atime_extra = bb.get_uint32_le();
        m_create_time = bb.get_uint32_le();
        m_create_time_extra = bb.get_uint32_le();
        m_version_Hi  = bb.get_uint32_le();
    }
    else 
    {
        m_create_time = m_change_time;
    }

    m_node_size = inodeSize;
    m_file_size = m_size;
    int64_t sizeHigh = m_dir_ACL;
    m_file_size += (sizeHigh << 32);
}

auto INode::is_valid() -> bool
{
    //m_blkPointers size == 60
    bool flag = false;

    for(uint8_t i=0; i < 60; i++)
    {
        if(*(m_blk_pointers + i) > 0)
        {
            flag = true;
            break;
        }
    }

    return m_file_size > 0 && flag;
}

auto INode::use_extents() -> bool
{
    return (m_flag & 0x80000) == 0x80000;
}

auto INode::is_directory() -> bool
{
    return (m_file_mode & 0xF000) == 0x4000;
}
auto INode::is_file() -> bool
{
    return (m_file_mode & 0xF000) == 0x8000;
}
auto INode::is_soft_link() -> bool
{
    return (m_file_mode & 0xF000) == 0xA000;
}