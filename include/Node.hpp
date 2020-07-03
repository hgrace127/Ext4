#pragma once
#ifndef NODE_H

#include <string>
#include <stdint.h>

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
    Active,
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
    uint16_t m_filemode;
    long m_size;
    long m_allocSize;
    long m_iNodeNo;
    uint64_t m_refId;
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