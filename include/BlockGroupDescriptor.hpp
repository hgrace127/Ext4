#pragma once

#include <stdint.h>

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
    BlockGroupDescriptor(uint8_t buff[], int offset);
    auto IsEmpty() -> bool;
};