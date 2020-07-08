#pragma once

#include <stdint.h>

class BlockGroupDescriptor{
public:
    const static int Size = 0x20;
    uint16_t directory_count;
    uint16_t free_block_count;
    uint16_t free_inode_count;
    uint32_t start_addr_of_blk;
    uint32_t start_addr_of_blk_bitmap;
    uint32_t start_blk_addr_of_inode_bitmap;
    uint32_t start_blk_addr_of_inode_table;

public:
    BlockGroupDescriptor(uint8_t* buff, int offset);
    
    auto is_empty() -> bool;
};