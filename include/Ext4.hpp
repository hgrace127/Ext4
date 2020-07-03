#pragma once

#include <List>
#include <fstream>

#include "Node.hpp"
#include "INode.hpp"
#include "Ext4_Extent.hpp"
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
    auto initExt4() -> bool;
    auto makeBlkGroupDescriptorTable() -> std::vector<BlockGroupDescriptor>;
    auto makeRootNode() -> Node*;
    auto makeNode(DirectoryEntry* de) -> Node*;
    auto makeNode(INode* inode, std::string name, bool active) -> Node*;
    auto findiNode(uint32_t no) -> INode*;
    auto nodeStreamFrom(INode* inode, uint8_t* extentsBuffer, bool active) -> NodeStream*;
    auto buildExtentsFrom(INode* inode, uint8_t* extentsBuffer, long* expectedLogicalBlkNo, bool active) -> std::vector<Ext4_Extent*>*;
    auto makeEmptyRoot() -> Node*;

public:
    long m_size;
    int m_blockSize;
    int m_iNodeSize;
    Node* m_rootNode;
    std::ifstream* m_stream;

private:
    Superblock* m_superblock;
    std::vector<BlockGroupDescriptor> m_blkGroupDescTable;
    std::vector<Extent> m_indirectExtents;
    int m_blockGroupCount;
    int m_iNodePerBlock;
    int m_iNodeBlockCount;
    long m_startAddress;
    bool m_isLive;
    bool m_isValid;
};