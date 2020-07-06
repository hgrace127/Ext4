#pragma once

#include <stdint.h>

class Superblock
{
public:
    Superblock(uint8_t* b, int offset = 0);
    auto blk_group_desc_size() -> int;
    auto is_valid() -> bool;
    auto active_inode_no() -> unsigned int;

public:
    uint32_t  m_iNodeCnt;
    uint32_t  m_blkCnt;
    uint32_t  m_reservedBlkCnt;
    uint32_t  m_freeBlkCnt;
    uint32_t  m_freeiNodeCnt;
    uint32_t  m_firstDataBlk;
    uint32_t  m_logBlkSize;
    uint32_t  m_logFragmentationSize;
    uint32_t  m_blkPerGroup;
    uint32_t  m_fragmentationPerGroup;
    uint32_t  m_iNodePerGroup;
    uint32_t  m_mTime;          // last mount time
    uint32_t  m_wTime;          // super block update time
    uint16_t  m_mountCnt;
    uint16_t  m_nextMountCnt;
    uint16_t  m_magicSignature;
    uint16_t  m_state;
    uint16_t  m_errors;
    uint16_t  m_minorVersion;
    uint32_t  m_lastConsistencyCheckTime;
    uint32_t  m_checkInterval;
    uint32_t  m_createOS;
    uint32_t  m_majorVersion;
    uint16_t  m_UID;                    //  UID for reserved block
    uint16_t  m_GID;                    //  GID for reserved block
    uint32_t  m_firstNonReservediNode;  //  GID for reserved block
    uint16_t  m_iNodeSize;
    uint16_t  m_blkGroupNo;
    uint32_t  m_compatibleFeatureFlags;
    uint32_t  m_incompatibleFeatureFlags;
    uint32_t  m_readOnlyFeatureFlags;
    uint8_t   m_UUID[16];
    uint8_t   m_volumeName[16];
    uint8_t   m_lastMountedPath[64];
    uint32_t  m_algorithmUsageBitmap;
    uint8_t   m_preAllocatedBlkCount;
    uint8_t   m_preAllocatedDirBlkCount;
    uint16_t  m_padding1;
    uint8_t   m_journalUUID[16];
    uint32_t  m_journaliNodeNo;
    uint32_t  m_journalDevice;
    uint32_t  m_journalOrphaniNodeList;
    uint8_t   m_hashSeed[16];
    uint8_t   m_definedHashVersion;
    uint8_t   m_padding2;
    uint16_t  m_padding3;
    uint32_t  m_defaultMountOption;
    uint32_t  m_firstMetaBlock;
    // reserved 760 bytes
};