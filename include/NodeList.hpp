#pragma once
#ifndef NODELIST_H
#include <vector>

#include "Node.hpp"

class Node;
class NodeList{
public:
    NodeList();
public:
    int m_count;
private:
    int m_activeNodeIndex;
    std::vector<Node*>* m_nodes;
    Node* m_owner; // parent node를 말하는 듯
};

#endif // NODELIST_H