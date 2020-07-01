#include <iostream>
#include <fstream>
#include <cstring>
#include "develop/include/byte_buffer2.hpp"
#include "develop/include/endian_swap.hpp"

class Superblock{
    public :
    
        bool IsValid(){
            return magicSignature == 0xef53 && iNodeSize != 0 && blkPerGroup !=0;
        };
        
        // uint32_t : uint32_t
        // uint16_t : uint16_t
        // uint8_t : uint8_t
        
        Superblock(uint8_t* b, int offset = 0){ 
            ByteBuffer2 bb(b, offset, 1024, true);
            // ByteBuffer2 bb_ushort;
            std::cout <<"11" <<'\n';
            iNodeCnt                            = bb.get_uint32_le();  // 총 아이노드 수
            reservedBlkCnt                      = bb.get_uint32_le();  // 총 블록 수
            freeBlkCnt                          = bb.get_uint32_le(); 
            freeiNodeCnt                        = bb.get_uint32_le(); 
            firstDataBlk                        = bb.get_uint32_le(); 
            logBlkSize                          = bb.get_uint32_le(); 
            logFragmentationSize                = bb.get_uint32_le(); 
            blkPerGroup                         = bb.get_uint32_le();   // 그룹 당 블록 수 ?
            fragmentationPerGroup               = bb.get_uint32_le();   
            iNodePerGroup                       = bb.get_uint32_le();   // 그룹 당 아이노드 ?
            mTime                               = bb.get_uint32_le(); 
            wTime                               = bb.get_uint32_le(); 
            std::cout <<"22" <<'\n';
            mountCnt                            = bb.get_uint16_le(); 
            nextMountCnt                        = bb.get_uint16_le(); 
            magicSignature                      = bb.get_uint16_le(); //Debug.Assert(magicSignature == 0xef53);
            state                               = bb.get_uint16_le(); 
            errors                              = bb.get_uint16_le(); 
            minorVersion                        = bb.get_uint16_le(); 
            lastConsistencyCheckTime            = bb.get_uint32_le();
            checkInterval                       = bb.get_uint32_le();
            createOS                            = bb.get_uint32_le();
            majorVersion                        = bb.get_uint32_le();
            UID                                 = bb.get_uint16_le();
            GID                                 = bb.get_uint16_le();
            firstNonReservediNode               = bb.get_uint32_le();
            iNodeSize                           = bb.get_uint16_le();   // 아이노드 사이즈
            blkGroupNo                          = bb.get_uint16_le();   // 수퍼블록의 블록 그룹 번호
            compatibleFeatureFlags              = bb.get_uint32_le();
            incompatibleFeatureFlags            = bb.get_uint32_le();
            readOnlyFeatureFlags                = bb.get_uint32_le();
            std::cout <<"22" <<'\n';

            // UUID                                = new uint8_t[16];
            // memcpy(UUID, b+offset+104, 16); // ?
            // volumeName                          = new uint8_t[16];
            // memcpy(volumeName, b+offset+120, 16);
            bb.destroy();


        }

        int BlkGroupDescSize(){
            return (incompatibleFeatureFlags & 0x80) == 0x80 ? padding3 : 0x20;
        }

        uint32_t ActiveiNodeNo(){
            return iNodeCnt - freeiNodeCnt;
        }
        

    
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