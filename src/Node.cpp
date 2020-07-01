#include <iostream>

using namespace std;

enum NodeType{
    None, 
    File, 
    Directory, 
    HardLink,
    SoftLink
};

enum NodeState
{
    Active, // Active 우선 구현
    Deleted, 
    Unused
};

enum NodeAttr
{
    Normal,
    Container,
    ContainerPart,
    RecycleBin,
    Virtual,
    Recovered
};

class Node{
    public:
        long m_uid;
        long m_gid;
        unsigned short m_filemode;
        long m_size;
        long m_allocSize;
        unsigned long m_refId;
        int m_dumpId;

    private:
        string m_absolutePath; // 절대경로
        string m_name; 
        string m_extension; // 확장자
        string m_partitionName;


};