#include <iostream>
#include <fstream>
#include <cstring>

#include "Superblock.hpp"
#include "develop/include/byte_buffer2.hpp"
#include "develop/include/endian_swap.hpp"

// uint32_t : uint32_t
// uint16_t : uint16_t
// uint8_t : uint8_t

Superblock::Superblock(uint8_t* b, int offset)
{ 
    ByteBuffer2 bb(b, offset, 1024, true);
    // ByteBuffer2 bb_ushort;
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

    UUID                                = new uint8_t[16];
    memcpy(UUID, b+offset+104, 16); // ?
    volumeName                          = new uint8_t[16];
    memcpy(volumeName, b+offset+120, 16);
}

auto Superblock::IsValid() -> bool
{
    return magicSignature == 0xef53 && iNodeSize != 0 && blkPerGroup !=0;
};


auto Superblock::BlkGroupDescSize() -> int
{
    return (incompatibleFeatureFlags & 0x80) == 0x80 ? padding3 : 0x20;
}

auto Superblock::ActiveiNodeNo() -> uint32_t
{
    return iNodeCnt - freeiNodeCnt;
}
