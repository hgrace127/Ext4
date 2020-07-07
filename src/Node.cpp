#include <iostream>
#include <vector>
#include <string>
#include <assert.h>

#include "Node.hpp"

using namespace std;

Node::Node(NodeType t)
{
    Node("/", t, nullptr, NodeAttr::Normal);
}

Node::Node(string name, NodeType t, NodeStream* stream, NodeAttr attr)
{  
    Node(name, t, NodeState::Active, stream, attr, 0L);
}

Node::Node(string name, NodeType t, NodeState s, NodeStream* stream, NodeAttr attr, long allocSize)
{
    m_state = s;
    m_node_type = t;
    m_attr   = attr;
    m_is_root = false;

    assert(is_hard_link() == false);

    m_absolute_path = name.rfind("/", 0) == 0 ? name : "/" + name;
    m_name = name;

    m_stream   = stream != nullptr ? stream : new NodeStream();
    m_allocSize = allocSize > 0 ? allocSize : m_stream->alloc_length();
    m_size = m_allocSize;
    m_children  = new NodeList(this, true);
    m_parent    = nullptr;
    m_check     = false;

    m_signature_matching = FileSignatureMatching::None;
}

auto Node::is_directory() -> bool
{
    return m_node_type == NodeType::Directory ? true : false;
}

auto Node::is_hard_link() -> bool
{
    return m_node_type == NodeType::HardLink;
}

auto Node::set_absolute_path(string value) -> void
{
    m_absolute_path = value;
}

auto Node::set_check(bool value) -> void
{
    if(m_check == value)
        return;

    m_check = value;
    update_child_check(this, m_check);
    update_parent_check(this);
}

auto Node::update_child_check(Node *n, bool check) -> void 
{
    Node* c;
    
    for(int i=0; i < n->m_children->m_nodes.size(); i++)
    {
        c = n->m_children->m_nodes[i];
        c->m_check = check;

        update_child_check(c, check);
    }
}
auto Node::update_parent_check(Node *n) -> void 
{
    while(true)
    {
        if(n == nullptr || n->m_parent == n)
            return;

            Node* parent = n->m_parent;
            update_check_by_children(parent);
            n = parent;
    }
}

auto Node::update_check_by_children(Node *n) -> void 
{
    if (n == nullptr || n->m_children->m_count == 0)
        return;

    int checkCount = 0;
    int unCheckCount = 0;
    int showCount = 0;
    Node* c;

    for(int i=0; i < n->m_children->m_nodes.size(); i++)
    {
        c = n->m_children->m_nodes[i];

        if (c->m_check == true)
            checkCount++;

        else if(c->m_check == false)
            unCheckCount++;

        showCount++;
    }
    if (checkCount == showCount)
        n->m_check = true;
    
    else if (unCheckCount == showCount)
        n->m_check = false;
    
    else
        n->m_check = false; // C#에서는 null을 넣음
}

auto Node::is_container() -> bool
{
    return m_attr == NodeAttr::Container;
}

auto Node::is_expandable() -> bool
{
    return (is_directory() || is_container()) && m_name != "." && m_name != "..";
}

auto Node::update_child() -> void
{
    for (auto c : m_children->m_nodes)
    {
        if (c->m_parent == nullptr)
            continue;

        c->m_absolute_path = c->m_parent->m_absolute_path + "/" + c->m_name;
        if (c->is_directory() || c->is_container())
            c->update_child();
    }
}