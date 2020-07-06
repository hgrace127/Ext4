#pragma once

#include <stdint.h>
#include <string>
#include <vector>
class INode
{
public:
    INode(uint8_t* b, long offset, long inodeSize, long imageOffset);
    
    auto is_directory() -> bool;
    auto is_file() -> bool;
    auto is_valid() -> bool;
    auto is_soft_link() -> bool;
    auto use_extents() -> bool;

public:
    uint16_t   m_file_mode;
    uint16_t   m_UID;         // user id
    uint32_t   m_size;
    uint32_t   m_access_time;
    uint32_t   m_change_time;  // NOTICE: inode change time
    uint32_t   m_modify_time;  // file change time
    uint32_t   m_delete_time;  // file "delete" time
    uint16_t   m_GID;
    uint16_t   m_link_count;
    uint32_t   m_blk_count;
    uint32_t   m_flag;
    uint32_t   m_os_desc1;
    uint8_t*   m_blk_pointers; // NOTICE Ext3: 4 * 15, Ext4: 12 * 5
    uint32_t   m_generation_number;
    uint32_t   m_file_ACL;
    uint32_t   m_dir_ACL;
    uint32_t   m_blk_addr_of_fragmentation;
    uint8_t*   m_os_desc2;

// extra fields for Ext4
    uint16_t   m_extrai_size;
    uint16_t   m_chksum_Hi;          // crc32c(uuid+inum+inode) BE 
    uint32_t   m_ctime_extra;        // extra Change time       (nanoSec << 2 | epoch)
    uint32_t   m_mtime_extra;        // extra Modification time (nanoSec << 2 | epoch)
    uint32_t   m_atime_extra;        // extra Access time       (nanoSec << 2 | epoch)
    uint32_t   m_create_time;        // File Creation time
    uint32_t   m_create_time_extra;   // extra FileCreationtime  (nanoSec << 2 | epoch)
    uint32_t   m_version_Hi;         // high 32 bits for 64-bit version

    int  m_node_size;
    int64_t m_address;
    int64_t m_No;
    int64_t m_file_size;
    std::string m_hex_repr;
    std::string m_hash_value;
};