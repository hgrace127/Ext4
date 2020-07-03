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
        m_activeNodeIndex = 0;

        //param node가 m_nodes에 있는지 확인하고 없으면 add

        vector<Node*>::iterator it = find(m_nodes->begin(), m_nodes->end(), node);
        if(it != m_nodes->end())
        {
            m_nodes->push_back(node);
        }
    }
}

    