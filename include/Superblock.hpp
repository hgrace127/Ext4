#pragma once

#include <stdint.h>

class Superblock
{
public:
    Superblock();
    Superblock(uint8_t* b, int offset = 0);

public:
    uint32_t  iNodeCnt;
    uint32_t  blkCnt;
    uint32_t  reservedBlkCnt;
    uint32_t  freeBlkCnt;
    uint32_t  freeiNodeCnt;
    uint32_t  firstDataBlk;
    uint32_t  logBlkSize;
    uint32_t  logFragmentationSize;
    uint32_t  blkPerGroup;
    uint32_t  fragmentationPerGroup;
    uint32_t  iNodePerGroup;
    uint32_t  mTime;          // last mount time
    uint32_t  wTime;          // super block update time
    uint16_t mountCnt;
    uint16_t nextMountCnt;
    uint16_t magicSignature;
    uint16_t state;
    uint16_t errors;
    uint16_t minorVersion;
    uint32_t  lastConsistencyCheckTime;
    uint32_t  checkInterval;
    uint32_t  createOS;
    uint32_t  majorVersion;
    uint16_t UID;                    //  UID for reserved block
    uint16_t GID;                    //  GID for reserved block
    uint32_t  firstNonReservediNode;  //  GID for reserved block
    uint16_t iNodeSize;
    uint16_t blkGroupNo;
    uint32_t  compatibleFeatureFlags;
    uint32_t  incompatibleFeatureFlags;
    uint32_t  readOnlyFeatureFlags;
    uint8_t* UUID;
    uint8_t* volumeName;
    uint8_t* lastMountedPath;
    uint32_t  algorithmUsageBitmap;
    uint8_t   preAllocatedBlkCount;
    uint8_t   preAllocatedDirBlkCount;
    uint16_t padding1;
    uint8_t* journalUUID;
    uint32_t  journaliNodeNo;
    uint32_t  journalDevice;
    uint32_t  journalOrphaniNodeList;
    uint8_t* hashSeed;
    uint8_t   definedHashVersion;
    uint8_t   padding2;
    uint16_t padding3;
    uint32_t  defaultMountOption;
    uint32_t  firstMetaBlock;
    // reserved 760 bytes
};