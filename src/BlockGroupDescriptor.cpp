#include "BlockGroupDescriptor.hpp"
#include "byte_buffer2.hpp"

BlockGroupDescriptor::BlockGroupDescriptor(uint8_t buff[], int offset){
    ByteBuffer2 bb(buff, 0, 0x20, true);

    start_addr_of_blk_bitmap = bb.get_uint32_le(offset + 0);
    start_blkAddr_of_iNode_bitmap = bb.get_uint32_le(offset + 4);
    start_blkAddr_of_iNode_table = bb.get_uint32_le(offset + 8);
    free_block_count = bb.get_uint16_le(offset + 12);
    free_inode_count = bb.get_uint16_le(offset + 14);
    directory_count = bb.get_uint16_le(offset + 16);
}

auto BlockGroupDescriptor::is_empty() -> bool
{
    return start_addr_of_blk_bitmap == 0;
}