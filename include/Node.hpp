#pragma once
#ifndef NODE_H

#include <string>

#include "NodeList.hpp"

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

class NodeList;
class Node
{
public:
    Node();
    auto IsDirectory() -> bool;

public:
    long m_uid;
    long m_gid;
    unsigned short m_filemode;
    long m_size;
    long m_allocSize;
    long m_iNodeNo;
    unsigned long m_refId;
    int m_dumpId;
    NodeList* m_children;
    Node* m_parent;
    NodeAttr m_nodeAttr;
private:
    std::string m_absolutePath; // 절대경로
    std::string m_name; 
    std::string m_extension; // 확장자
    std::string m_partitionName;
    long m_offset;
    NodeType m_nodeType;
};
#endif // NODE_H