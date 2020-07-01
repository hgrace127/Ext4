#include "byte_buffer2.hpp"

class BlockGroupDescriptor{
public:
    const static int Size = 0x20;

    unsigned int startAddrOfBlk;
    unsigned int startAddrOfBlkBitmap;
    unsigned int startBlkAddrOfiNodeBitmap;
    unsigned int startBlkAddrOfiNodeTable;

    unsigned short freeBlockCount;
    unsigned short freeiNodeCount;
    unsigned short dirCount;

public:
    BlockGroupDescriptor(uint8_t buff[], int offset){
        ByteBuffer2 bb(buff, 0, 0x20, true);

        startAddrOfBlkBitmap      = bb.get_uint32_le(offset + 0);       
        startBlkAddrOfiNodeBitmap = bb.get_uint32_le(offset + 4); 
        startBlkAddrOfiNodeTable  = bb.get_uint32_le(offset + 8); 
        freeBlockCount = bb.get_uint32_le(offset + 12); 
        freeiNodeCount = bb.get_uint32_le(offset + 14); 
        dirCount       = bb.get_uint32_le(offset + 16); 
    }
};