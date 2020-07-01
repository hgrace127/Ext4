#include <vector>

#include "Node.cpp"
#include "Superblock.cpp"
#include "BlockGroupDescriptor.cpp"

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

class Extent{
public:
    long m_LOffset;
    long m_POffset;
    long m_count;

private:

public:
    Extent() {}

};


class Ext4{

public:
    long m_size;
    int m_blockSize;
    int m_iNodeSize;

    Node m_rootNode;   

private:
    Superblock m_superblock;
    vector<BlockGroupDescriptor> m_blkGroupDescTable;
    int m_blockGroupCount;
    int m_iNodePerBlock; // 한 블록이 들어갈 수 있는 INode 갯수
    int m_iNodeBlockCount;

    bool m_isLive;
    long m_startAddress;

public:
    auto BuildFileSystem() {}



};