#include <iostream>
#include <fstream>
#include <cstring>

#include "Superblock.hpp"
#include "byte_buffer2.hpp"

Superblock::Superblock(uint8_t* b, int offset)
{ 
    ByteBuffer2 bb(b, offset, 0x400, false);
    m_inode_cnt                            = bb.get_uint32_le();  // 총 아이노드 수
    m_blk_cnt                              = bb.get_uint32_le();
    m_reserved_blk_cnt                     = bb.get_uint32_le();  // 총 블록 수
    m_free_blk_cnt                         = bb.get_uint32_le();
    m_free_inode_cnt                       = bb.get_uint32_le();
    m_first_data_blk                       = bb.get_uint32_le();
    m_log_blk_size                         = bb.get_uint32_le();
    m_log_fragmentation_size               = bb.get_uint32_le();
    m_blk_per_group                        = bb.get_uint32_le();   // 그룹 당 블록 수 ?
    m_fragmentation_per_group              = bb.get_uint32_le();
    m_inode_per_group                      = bb.get_uint32_le();   // 그룹 당 아이노드 ?
    m_mtime                                = bb.get_uint32_le();
    m_wtime                                = bb.get_uint32_le();
    m_mount_cnt                            = bb.get_uint16_le();
    m_next_mount_cnt                       = bb.get_uint16_le();
    m_magic_signature                      = bb.get_uint16_le(); //Debug.Assert(magicSignature == 0xef53);
    m_state                                = bb.get_uint16_le();
    m_errors                               = bb.get_uint16_le();
    m_minor_version                        = bb.get_uint16_le();
    m_last_consistency_checktime           = bb.get_uint32_le();
    m_check_interval                       = bb.get_uint32_le();
    m_create_OS                            = bb.get_uint32_le();
    m_major_version                        = bb.get_uint32_le();
    m_UID                                 = bb.get_uint16_le();
    m_GID                                 = bb.get_uint16_le();
    m_first_non_reserved_inode               = bb.get_uint32_le();
    m_inode_size                           = bb.get_uint16_le();   // 아이노드 사이즈
    m_blk_group_no                          = bb.get_uint16_le();   // 수퍼블록의 블록 그룹 번호
    m_compatible_feature_flags              = bb.get_uint32_le();
    m_incompatible_feature_flags            = bb.get_uint32_le();
    m_read_only_feature_flags                = bb.get_uint32_le();
    memcpy(m_UUID, b+offset+104, 16);
    memcpy(m_volume_name, b+offset+120, 16);
    memcpy(m_last_mounted_path, b+offset+136, 64);
    m_algorithm_usage_bitmap     = bb.get_uint32_le(200);
    m_pre_allocated_blk_count     = bb[204];
    m_pre_allocated_dir_blk_count  = bb[205];
    m_padding1                 = bb.get_uint16_le(206);
    memcpy(m_journal_UUID, b+offset+208, 16);
    m_journal_inode_no           = bb.get_uint32_le(224);
    m_journal_device            = bb.get_uint32_le(228);
    m_journal_orphan_inode_list   = bb.get_uint32_le(232);
    memcpy(m_hash_seed, b+offset+236, 16);
    m_defined_hash_version       = b[252];
    m_padding2                 = b[253];
    m_padding3                 = bb.get_uint16_le(254);
    m_default_mount_option       = bb.get_uint32_le(256);
    m_first_meta_block           = bb.get_uint32_le(260);
}

auto Superblock::blk_group_desc_size() -> int
{
    return (m_incompatible_feature_flags & 0x80) == 0x80
    ? m_padding3
    : 0x20;
}

auto Superblock::active_inode_no() -> unsigned int
{
    return m_inode_cnt - m_free_inode_cnt;
}

auto Superblock::is_valid() -> bool
{
    return m_magic_signature == 0xef53 && m_inode_size != 0 && m_blk_per_group != 0;
}