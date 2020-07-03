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

Node::Node(string name, NodeType t, NodeStream* stream = nullptr, NodeAttr attr = NodeAttr::Normal)
{  
    Node(name, t, NodeState::Active, stream, attr, 0L);
}

Node::Node(string name, NodeType t, NodeState s, NodeStream* stream, NodeAttr attr = NodeAttr::Normal, long allocSize = 0)
{
    m_state = s;
    m_type  = t;
    m_attr   = attr;

    assert(IsHardLink() == false);

    m_absolutePath = name.rfind("/", 0) == 0 ? name : "/" + name;
    m_name = name;

    m_stream   = stream != nullptr ? stream : new NodeStream();
    m_allocSize = allocSize > 0 ? allocSize : m_stream->AllocLength();
    m_children  = new NodeList(this, true);
    m_parent    = nullptr;
    m_check     = false;

    m_SignatureMatching = FileSignatureMatching::Non;
}

auto Node::IsDirectory() -> bool
{
    return m_nodeType == NodeType::Directory ? true : false;
}

auto Node::IsHardLink() -> bool
{
    return m_type == NodeType::HardLink;
}

auto Node::setAbsolutePath(string value) -> void
{
    m_absolutePath = value;
}

auto Node::setCheck(bool value) -> void
{
    if(m_check == value)
        return;

    m_check = value;
    UpdateChildCheck(this, m_check);
    UpdateParentCheck(this);
}

auto Node::UpdateChildCheck(Node *n, bool check) -> void 
{
    Node* c;
    for(int i=0; i < n->m_children->m_nodes->size(); i++)
    {
        c = n->m_children->m_nodes->at(i);
        c->m_check = check;

        UpdateChildCheck(c, check);
    }
}
auto Node::UpdateParentCheck(Node *n) -> void 
{
    while(true)
    {
        if(n == nullptr || n->m_parent == n)
            return;

            Node* parent = n->m_parent;
            UpdateCheckByChildren(parent);
            n = parent;
    }
}

auto Node::UpdateCheckByChildren(Node *n) -> void 
{
    if (n == nullptr || n->m_children->m_count == 0)
        return;

    int checkCount = 0;
    int unCheckCount = 0;
    int showCount = 0;
    Node* c;

    for(int i=0; i < n->m_children->m_nodes->size(); i++)
    {
        c = n->m_children->m_nodes->at(i);

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