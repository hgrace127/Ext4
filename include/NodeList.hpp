#pragma once
#ifndef NODELIST_H
#include <vector>

#include "Node.hpp"

class Node;
class NodeList{
public:
    NodeList();
    NodeList(Node* node, bool isOwner);

public:
    int m_count;
    std::vector<Node*>* m_nodes;

private:
    int m_active_node_index;
    Node* m_owner; // parent node?
};

#endif // NODELIST_H