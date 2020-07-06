#pragma once

#include <List>
#include <fstream>

#include "Node.hpp"
#include "INode.hpp"
#include "Ext4Extent.hpp"
#include "DirectoryEntry.hpp"
#include "NodeStream.hpp"
#include "Superblock.hpp"
#include "BlockGroupDescriptor.hpp"

enum BlockType
{
    Boot,
    Super,
    GroupDesc,
    BlockBitmap,
    InodeBitmap,
    InodeTable,
    Data,
    Invalid
};


class Ext4
{
public:
    Ext4(std::ifstream* stream, long startAddress, bool isLive);

private:
    auto init_ext4() -> bool;
    auto make_blk_group_descriptor_table() -> std::vector<BlockGroupDescriptor>;
    auto make_root_node() -> Node*;
    auto make_node(DirectoryEntry* de) -> Node*;
    auto make_node(INode* inode, std::string name, bool active) -> Node*;
    auto find_inode(uint32_t no) -> INode*;
    auto node_stream_from(INode* inode, uint8_t* extentsBuffer, bool active) -> NodeStream*;
    auto build_extents_from(INode* inode, uint8_t* extentsBuffer, long* expectedLogicalBlkNo, bool active) -> std::vector<Ext4Extent*>;
    auto make_empty_root() -> Node*;

public:
    long m_size;
    int m_block_size;
    int m_inode_size;
    Node* m_root_node;
    std::ifstream* m_stream;

private:
    Superblock* m_superblock;
    std::vector<BlockGroupDescriptor> m_blk_group_desc_table;
    std::vector<Extent> m_indirect_extents;
    int m_block_group_count;
    int m_inode_per_block;
    int m_inode_block_count;
    long m_start_address;
    bool m_is_live;
    bool m_is_valid;
};