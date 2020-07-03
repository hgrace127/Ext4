#pragma once

#include <stdint.h>

class BlockGroupDescriptor{
public:
    const static int Size = 0x20;
    uint32_t startAddrOfBlk;
    uint32_t startAddrOfBlkBitmap;
    uint32_t startBlkAddrOfiNodeBitmap;
    uint32_t startBlkAddrOfiNodeTable;
    uint16_t freeBlockCount;
    uint16_t freeiNodeCount;
    uint16_t dirCount;

public:
    BlockGroupDescriptor(uint8_t buff[], int offset);
    auto IsEmpty() -> bool;
};