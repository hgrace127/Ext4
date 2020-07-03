#include "iNode.hpp"
#include "byte_buffer2.hpp"

using namespace std;

INode::INode(uint8_t* b, long offset, long inodeSize, long imageOffset)
{
    ByteBuffer2 bb(b, offset, inodeSize, false);

    m_address = imageOffset;
    m_hexRepr = "";

    m_fileMode    = bb.get_uint16_le(); // 0         
    m_UID         = bb.get_uint16_le(); // 2
    m_sizeInBytes = bb.get_uint32_le(); // 4
    m_accessTime  = bb.get_uint32_le(); // 8
    m_changeTime  = bb.get_uint32_le(); // 12  NOTICE: inode change time
    m_modifyTime  = bb.get_uint32_le(); // 16          file change time
    m_deleteTime  = bb.get_uint32_le(); // 20
    m_GID         = bb.get_uint16_le(); // 24
    m_linkCount   = bb.get_uint16_le(); // 26
    m_blkCount    = bb.get_uint32_le(); // 28
    m_flag        = bb.get_uint32_le(); // 32
    m_osDesc1     = bb.get_uint32_le(); // 36
    m_blkPointers = bb.get_bytes(60);   // 40  NOTICE: in Ext3 4 * 15 = 60
    m_generationNumber
                  = bb.get_uint32_le(); // 100
    m_fileACL     = bb.get_uint32_le(); // 104
    m_dirACL      = bb.get_uint32_le(); // 108
    m_blkAddrOfFragmentation
                  = bb.get_uint32_le(); // 112
    m_osDesc2     = bb.get_bytes(12);   // 116

    if (UsesExtents())
    {
        m_extraiSize = bb.get_uint16_le(); // extra fields
        m_chksumHi   = bb.get_uint16_le();
        m_cTimeExtra = bb.get_uint32_le();
        m_mTimeExtra = bb.get_uint32_le();
        m_aTimeExtra = bb.get_uint32_le();
        m_createTime = bb.get_uint32_le();
        m_createTimeExtra = bb.get_uint32_le();
        m_versionHi  = bb.get_uint32_le();
    }
    else 
    {
        m_createTime = m_changeTime;
    }

    m_nodeSize = inodeSize;
    m_fileSize = m_sizeInBytes;
    int64_t sizeHigh = m_dirACL;
    m_fileSize += (sizeHigh << 32);
}

auto INode::IsValid() -> bool
{
    //m_blkPointers size == 60
    bool flag = false;

    for(uint8_t i=0; i < 60; i++)
    {
        if(*(m_blkPointers + i) > 0)
        {
            flag = true;
            break;
        }
    }

    return m_fileSize > 0 && flag;
}

auto INode::UsesExtents() -> bool
{
    return (m_flag & 0x80000) == 0x80000;
}

auto INode::IsDir() -> bool
{
    return (m_fileMode & 0xF000) == 0x4000;
}
auto INode::IsFile() -> bool
{
    return (m_fileMode & 0xF000) == 0x8000;
}
auto INode::IsSoftLink() -> bool
{
    return (m_fileMode & 0xF000) == 0xA000;
}