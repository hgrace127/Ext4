#include <algorithm>

#include "NodeList.hpp"

using namespace std;

NodeList::NodeList() {}

NodeList::NodeList(Node *node, bool isOwner = true)
{
    if (isOwner)
    {
        m_owner = node;
    }
    else
    {
        m_active_node_index = 0;

        //param node가 m_nodes에 있는지 확인하고 없으면 add
        vector<Node*>::iterator it = find(m_nodes.begin(), m_nodes.end(), node);
        if(it != m_nodes.end())
        {
            m_nodes.push_back(node);
        }
    }
}

auto NodeList::add(Node* node) -> void
{
    Node* owner = m_owner;

    if (owner == nullptr)
        return;

    if (!owner->is_directory() && !owner->is_container())
        return;

    node->m_parent = owner;

    node->m_absolute_path = owner->m_is_root ? "/" + node->m_name : owner->m_absolute_path + "/" + node->m_name;

    if (node->is_directory() || node->is_container())
        node->update_child();

    m_nodes.push_back(node);
}
    