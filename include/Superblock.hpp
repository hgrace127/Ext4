#pragma once

#include <stdint.h>

class Superblock
{
public:
    Superblock(uint8_t* b, int offset = 0);
    auto blk_group_desc_size() -> int;
    auto is_valid() -> bool;
    auto active_inode_no() -> unsigned int;

public:
    uint32_t  m_inode_cnt;
    uint32_t  m_blk_cnt;
    uint32_t  m_reserved_blk_cnt;
    uint32_t  m_free_blk_cnt;
    uint32_t  m_free_inode_cnt;
    uint32_t  m_first_data_blk;
    uint32_t  m_log_blk_size;
    uint32_t  m_log_fragmentation_size;
    uint32_t  m_blk_per_group;
    uint32_t  m_fragmentation_per_group;
    uint32_t  m_inode_per_group;
    uint32_t  m_mtime;          // last mount time
    uint32_t  m_wtime;          // super block update time
    uint16_t  m_mount_cnt;
    uint16_t  m_next_mount_cnt;
    uint16_t  m_magic_signature;
    uint16_t  m_state;
    uint16_t  m_errors;
    uint16_t  m_minor_version;
    uint32_t  m_last_consistency_checktime;
    uint32_t  m_check_interval;
    uint32_t  m_create_OS;
    uint32_t  m_major_version;
    uint16_t  m_UID;                    //  UID for reserved block
    uint16_t  m_GID;                    //  GID for reserved block
    uint32_t  m_first_non_reserved_inode;  //  GID for reserved block
    uint16_t  m_inode_size;
    uint16_t  m_blk_group_no;
    uint32_t  m_compatible_feature_flags;
    uint32_t  m_incompatible_feature_flags;
    uint32_t  m_read_only_feature_flags;
    uint8_t   m_UUID[16];
    uint8_t   m_volume_name[16];
    uint8_t   m_last_mounted_path[64];
    uint32_t  m_algorithm_usage_bitmap;
    uint8_t   m_pre_allocated_blk_count;
    uint8_t   m_pre_allocated_dir_blk_count;
    uint16_t  m_padding1;
    uint8_t   m_journal_UUID[16];
    uint32_t  m_journal_inode_no;
    uint32_t  m_journal_device;
    uint32_t  m_journal_orphan_inode_list;
    uint8_t   m_hash_seed[16];
    uint8_t   m_defined_hash_version;
    uint8_t   m_padding2;
    uint16_t  m_padding3;
    uint32_t  m_default_mount_option;
    uint32_t  m_first_meta_block;
    // reserved 760 bytes
};