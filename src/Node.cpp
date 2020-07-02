#include <iostream>
#include <vector>

#include "Node.hpp"

Node::Node() {}

auto Node::IsDirectory() -> bool
{
    return m_nodeType == NodeType::Directory ? true : false;
}