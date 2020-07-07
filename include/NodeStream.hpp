#pragma once

#include <vector>
#include <fstream>

#include "Extent.hpp"

class NodeStream 
{
public:
    NodeStream();
    NodeStream(long offset, std::vector<Extent*> extents, long length);
    NodeStream(std::vector<Extent*> extents, bool compact, long actualLength);
    
    auto alloc_length() -> long;
    auto read(std::ifstream *stream, long offset, int size) -> uint8_t*;
    auto set_length(long val) -> void;

public:
    long m_offset;
    long m_actual_length;
    std::vector<Extent*> m_extents;
    std::ifstream* m_nullstream = nullptr;
};
