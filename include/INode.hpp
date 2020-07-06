#pragma once

#include <stdint.h>
#include <string>
#include <vector>
class INode
{
public:
    INode(uint8_t* b, long offset, long inodeSize, long imageOffset);
    auto is_valid() -> bool;
    auto use_extents() -> bool;
    auto is_directory() -> bool;
    auto is_file() -> bool;
    auto is_soft_link() -> bool;

public:
    uint16_t   m_fileMode;
    uint16_t   m_UID;         // user id
    uint32_t   m_size;
    uint32_t   m_accessTime;
    uint32_t   m_changeTime;  // NOTICE: inode change time
    uint32_t   m_modifyTime;  // file change time
    uint32_t   m_deleteTime;  // file "delete" time
    uint16_t   m_GID;
    uint16_t   m_linkCount;
    uint32_t   m_blkCount;
    uint32_t   m_flag;
    uint32_t   m_osDesc1;
    uint8_t*   m_blkPointers; // NOTICE Ext3: 4 * 15, Ext4: 12 * 5
    uint32_t   m_generationNumber;
    uint32_t   m_fileACL;
    uint32_t   m_dirACL;
    uint32_t   m_blkAddrOfFragmentation;
    uint8_t*   m_osDesc2;

// extra fields for Ext4
    uint16_t   m_extraiSize;
    uint16_t   m_chksumHi;          // crc32c(uuid+inum+inode) BE 
    uint32_t   m_cTimeExtra;        // extra Change time       (nanoSec << 2 | epoch)
    uint32_t   m_mTimeExtra;        // extra Modification time (nanoSec << 2 | epoch)
    uint32_t   m_aTimeExtra;        // extra Access time       (nanoSec << 2 | epoch)
    uint32_t   m_createTime;        // File Creation time
    uint32_t   m_createTimeExtra;   // extra FileCreationtime  (nanoSec << 2 | epoch)
    uint32_t   m_versionHi;         // high 32 bits for 64-bit version

    int  m_nodeSize;
    int64_t m_address;
    int64_t m_No;
    int64_t m_fileSize;
    std::string m_hexRepr;
    std::string m_hashValue;
};