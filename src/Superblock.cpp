using namespace std;
#include <string>
class Superblock{
    public :
    
        bool IsValid(){
            return magicSignature == 0xef53 && iNodeSize != 0 && blkPerGroup !=0;
        };
        
        // unsigned int : unsigned int
        // unsigned short : unsigned short
        // unsigned char : unsigned char
        
        void SuperBlock(unsigned char* b, int offset = 0){ 
            iNodeCnt                            = *reinterpret_cast<unsigned int*>(b + offset+0); 
            blkCnt                              = *reinterpret_cast<unsigned int*>(b + offset+4); 
            reservedBlkCnt                      = *reinterpret_cast<unsigned int*>(b + offset+8); 
            freeBlkCnt                          = *reinterpret_cast<unsigned int*>(b + offset+12); 
            freeiNodeCnt                        = *reinterpret_cast<unsigned int*>(b + offset+16); 
            firstDataBlk                    = *reinterpret_cast<unsigned int*>(b + offset+20); 
            logBlkSize                      = *reinterpret_cast<unsigned int*>(b + offset+24); 
            logFragmentationSize            = *reinterpret_cast<unsigned int*>(b + offset+28); 
            blkPerGroup                 = *reinterpret_cast<unsigned int*>(b + offset+32); 
            fragmentationPerGroup                 = *reinterpret_cast<unsigned int*>(b + offset+36); 
            iNodePerGroup                 = *reinterpret_cast<unsigned int*>(b + offset+40); 
            mTime                 = *reinterpret_cast<unsigned int*>(b + offset+44); 
            wTime                 = *reinterpret_cast<unsigned int*>(b + offset+48); 
            mountCnt                 = *reinterpret_cast<unsigned int*>(b + offset+52); 
            nextMountCnt                 = *reinterpret_cast<unsigned int*>(b + offset+54); 
            magicSignature                 = *reinterpret_cast<unsigned int*>(b + offset+56); 
            state                 = *reinterpret_cast<unsigned int*>(b + offset+58); 
            errors                 = *reinterpret_cast<unsigned int*>(b + offset+60); 
            minorVersion                 = *reinterpret_cast<unsigned int*>(b + offset+62); 
            lastConsistencyCheckTime                 = *reinterpret_cast<unsigned int*>(b + offset+64); 
            checkInterval                 = *reinterpret_cast<unsigned int*>(b + offset+68); 
            createOS                 = *reinterpret_cast<unsigned int*>(b + offset+72);
            majorVersion                 = *reinterpret_cast<unsigned int*>(b + offset+76); 
            UID                 = *reinterpret_cast<unsigned int*>(b + offset+80); 
            GID                 = *reinterpret_cast<unsigned int*>(b + offset+82); 
            firstNonReservediNode                 = *reinterpret_cast<unsigned int*>(b + offset+84); 
            iNodeSize                 = *reinterpret_cast<unsigned int*>(b + offset+88);
            blkGroupNo                 = *reinterpret_cast<unsigned int*>(b + offset+90); 
            compatibleFeatureFlags                 = *reinterpret_cast<unsigned int*>(b + offset+92); 
            incompatibleFeatureFlags                 = *reinterpret_cast<unsigned int*>(b + offset+96); 
            readOnlyFeatureFlags                 = *reinterpret_cast<unsigned int*>(b + offset+100); 

        }

        int BlkGroupDescSize(){
            return (incompatibleFeatureFlags & 0x80) == 0x80 ? padding3 : 0x20;
        }

        unsigned int ActiveiNodeNo(){
            return iNodeCnt - freeiNodeCnt;
        }
        


        unsigned int  iNodeCnt;
        unsigned int  blkCnt;
        unsigned int  reservedBlkCnt;
        unsigned int  freeBlkCnt;
        unsigned int  freeiNodeCnt;
        unsigned int  firstDataBlk;
        unsigned int  logBlkSize;
        unsigned int  logFragmentationSize;
        unsigned int  blkPerGroup;
        unsigned int  fragmentationPerGroup;
        unsigned int  iNodePerGroup;
        unsigned int  mTime;          // last mount time
        unsigned int  wTime;          // super block update time
        unsigned short mountCnt;
        unsigned short nextMountCnt;
        unsigned short magicSignature;
        unsigned short state;
        unsigned short errors;
        unsigned short minorVersion;
        unsigned int  lastConsistencyCheckTime;
        unsigned int  checkInterval;
        unsigned int  createOS;
        unsigned int  majorVersion;
        unsigned short UID;                    //  UID for reserved block
        unsigned short GID;                    //  GID for reserved block
        unsigned int  firstNonReservediNode;  //  GID for reserved block
        unsigned short iNodeSize;
        unsigned short blkGroupNo;
        unsigned int  compatibleFeatureFlags;
        unsigned int  incompatibleFeatureFlags;
        unsigned int  readOnlyFeatureFlags;
        unsigned char* UUID;
        unsigned char* volumeName;
        unsigned char* lastMountedPath;
        unsigned int  algorithmUsageBitmap;
        unsigned char   preAllocatedBlkCount;
        unsigned char   preAllocatedDirBlkCount;
        unsigned short padding1;
        unsigned char* journalUUID;
        unsigned int  journaliNodeNo;
        unsigned int  journalDevice;
        unsigned int  journalOrphaniNodeList;
        unsigned char* hashSeed;
        unsigned char   definedHashVersion;
        unsigned char   padding2;
        unsigned short padding3;
        unsigned int  defaultMountOption;
        unsigned int  firstMetaBlock;
        // reserved 760 bytes

};