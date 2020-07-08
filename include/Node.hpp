#pragma once
#ifndef NODE_H

#include <string>
#include <stdint.h>

#include "NodeList.hpp"
#include "NodeStream.hpp"

enum class NodeType{
    None, 
    File, 
    Directory, 
    HardLink,
    SoftLink
};

enum class NodeState
{
    Active,
    Deleted, 
    Unused
};

enum class NodeAttr
{
    Normal,
    Container,
    ContainerPart,
    RecycleBin,
    Virtual,
    Recovered
};

enum class FileSignatureMatching
{
    None,
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
    
    
    auto is_container() -> bool;
    auto is_directory() -> bool;
    auto is_expandable() -> bool;
    auto is_hard_link() -> bool;
    auto set_absolute_path(std::string value) -> void;
    auto set_check(bool value) -> void;
    auto update_child() -> void;
    auto update_child_check(Node *n, bool check) -> void;
    auto update_parent_check(Node *n) -> void;
    auto update_check_by_children(Node *n) -> void;


public:
    long m_uid;
    long m_gid;
    uint16_t m_filemode;
    long m_size;
    long m_allocSize;
    long m_iNode_no;
    uint64_t m_ref_id;
    int m_dump_id;
    bool m_check;
    bool m_is_root;
    std::string m_absolute_path; // 절대경로
    std::string m_name;

    Node* m_parent;
    NodeAttr m_attr;
    NodeAttr m_node_attr;
    NodeList* m_children;
    NodeStream* m_stream;
    NodeState m_state;
    NodeType m_node_type;
    FileSignatureMatching m_signature_matching;
    
private:
    long m_offset;
    
    std::string m_extension; // 확장자
    std::string m_partition_name;
    
};
#endif // NODE_H