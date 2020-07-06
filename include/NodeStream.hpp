#pragma once

#include <vector>
#include <fstream>

#include "Extent.hpp"

class NodeStream 
{
public:
    NodeStream();
    NodeStream(std::vector<Extent*> extents, bool compact, long actualLength);
    auto set_length(long val) -> void;
    auto alloc_length() -> long;

public:
    long m_offset;
    long m_actual_length;
    std::vector<Extent*> m_extents;
    std::ifstream* m_nullstream = nullptr;
    // lock
};
