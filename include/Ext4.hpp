#pragma once

#include <vector>

#include "Node.hpp"
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
public :
    Ext4();

public:
    long m_size;
    int m_blockSize;
    int m_iNodeSize;
    Node m_rootNode;   

private:
    Superblock m_superblock;
    std::vector<BlockGroupDescriptor> m_blkGroupDescTable;
    int m_blockGroupCount;
    int m_iNodePerBlock;
    int m_iNodeBlockCount;
    long m_startAddress;
    bool m_isLive;
};