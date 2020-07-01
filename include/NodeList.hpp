#pragma once

#include <vector>

#include "Node.hpp"

class NodeList{
public:
    NodeList();
public:
    int m_count;
private:
    int m_activeNodeIndex;
    std::vector<Node>* m_nodes;
    Node* m_owner; // parent node를 말하는 듯
};