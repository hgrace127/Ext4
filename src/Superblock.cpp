#include <iostream>
#include <fstream>
#include <cstring>

#include "Superblock.hpp"
#include "byte_buffer2.hpp"

Superblock::Superblock(uint8_t* b, int offset)
{ 
    ByteBuffer2 bb(b, offset, 0x400, false);
    m_iNodeCnt                            = bb.get_uint32_le();  // 총 아이노드 수
    m_blkCnt                              = bb.get_uint32_le();
    m_reservedBlkCnt                      = bb.get_uint32_le();  // 총 블록 수
    m_freeBlkCnt                          = bb.get_uint32_le();
    m_freeiNodeCnt                        = bb.get_uint32_le();
    m_firstDataBlk                        = bb.get_uint32_le();
    m_logBlkSize                          = bb.get_uint32_le();
    m_logFragmentationSize                = bb.get_uint32_le();
    m_blkPerGroup                         = bb.get_uint32_le();   // 그룹 당 블록 수 ?
    m_fragmentationPerGroup               = bb.get_uint32_le();
    m_iNodePerGroup                       = bb.get_uint32_le();   // 그룹 당 아이노드 ?
    m_mTime                               = bb.get_uint32_le();
    m_wTime                               = bb.get_uint32_le();
    m_mountCnt                            = bb.get_uint16_le();
    m_nextMountCnt                        = bb.get_uint16_le();
    m_magicSignature                      = bb.get_uint16_le(); //Debug.Assert(magicSignature == 0xef53);
    m_state                               = bb.get_uint16_le();
    m_errors                              = bb.get_uint16_le();
    m_minorVersion                        = bb.get_uint16_le();
    m_lastConsistencyCheckTime            = bb.get_uint32_le();
    m_checkInterval                       = bb.get_uint32_le();
    m_createOS                            = bb.get_uint32_le();
    m_majorVersion                        = bb.get_uint32_le();
    m_UID                                 = bb.get_uint16_le();
    m_GID                                 = bb.get_uint16_le();
    m_firstNonReservediNode               = bb.get_uint32_le();
    m_iNodeSize                           = bb.get_uint16_le();   // 아이노드 사이즈
    m_blkGroupNo                          = bb.get_uint16_le();   // 수퍼블록의 블록 그룹 번호
    m_compatibleFeatureFlags              = bb.get_uint32_le();
    m_incompatibleFeatureFlags            = bb.get_uint32_le();
    m_readOnlyFeatureFlags                = bb.get_uint32_le();
    //m_UUID                                = new uint8_t[16];
    memcpy(m_UUID, b+offset+104, 16); // ?
    //m_volumeName                          = new uint8_t[16];
    memcpy(m_volumeName, b+offset+120, 16);
    
    //m_lastMountedPath = new uint8_t[64];
    memcpy(m_lastMountedPath, b+offset+136, 64);

    m_algorithmUsageBitmap = bb.get_uint32_le();
    m_preAllocatedBlkCount = bb[204];
    m_preAllocatedDirBlkCount = bb[205];
    m_padding1        = bb.get_uint16_le();

    //m_journalUUID = new uint8_t[16];
    memcpy(m_journalUUID, b+offset+208, 16);

    m_journaliNodeNo = bb.get_uint32_le();
    m_journalDevice            = bb.get_uint32_le();
    m_journalOrphaniNodeList   = bb.get_uint32_le();

    //m_hashSeed = new uint8_t[16];
    memcpy(m_hashSeed, b+offset+236, 16);

    m_definedHashVersion       = b[252];
    m_padding2                 = b[253];
    m_padding3                 = bb.get_uint16_le();
    m_defaultMountOption       = bb.get_uint32_le();
    m_firstMetaBlock           = bb.get_uint32_le();
}

auto Superblock::blk_group_desc_size() -> int
{
    return (m_incompatibleFeatureFlags & 0x80) == 0x80
    ? m_padding3
    : 0x20;
}

auto Superblock::active_inode_no() -> unsigned int
{
    return m_iNodeCnt - m_freeiNodeCnt;
}

auto Superblock::is_valid() -> bool
{
    return m_magicSignature == 0xef53 && m_iNodeSize != 0 && m_blkPerGroup != 0;
}