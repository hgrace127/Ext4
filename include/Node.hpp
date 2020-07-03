#pragma once
#ifndef NODE_H

#include <string>
#include <stdint.h>

#include "NodeList.hpp"
#include "NodeStream.hpp"

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

 enum FileSignatureMatching
{
    Non,
    Match,
    Mismatch,
    Bad,
    Unknown
};

class NodeList;
class Node
{
public:
    Node(NodeType t);
    Node(std::string name, NodeType t, NodeStream* stream, NodeAttr attr);
    Node(std::string name, NodeType t, NodeState s, NodeStream* stream, NodeAttr attr, long allocSize);
    auto IsDirectory() -> bool;
    auto IsHardLink() -> bool;
    auto setAbsolutePath(std::string value) -> void;
    auto setCheck(bool value) -> void;
    auto UpdateChildCheck(Node *n, bool check) -> void;
    auto UpdateParentCheck(Node *n) -> void;
    auto UpdateCheckByChildren(Node *n) -> void;

public:
    long m_uid;
    long m_gid;
    uint16_t m_filemode;
    long m_size;
    long m_allocSize;
    long m_iNodeNo;
    uint64_t m_refId;
    int m_dumpId;
    bool m_check;

    NodeList* m_children;
    Node* m_parent;
    NodeStream* m_stream;
    NodeAttr m_nodeAttr;
    NodeState m_state;
    NodeType m_type;
    NodeAttr m_attr;

    FileSignatureMatching m_SignatureMatching;
    
private:
    std::string m_absolutePath; // 절대경로
    std::string m_name; 
    std::string m_extension; // 확장자
    std::string m_partitionName;
    long m_offset;
    NodeType m_nodeType;
};
#endif // NODE_H