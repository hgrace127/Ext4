#pragma once

#include <fstream>
#include <stdint.h>

class Extent
{
public:
    Extent(std::ifstream* stream, long offset, long count);
    
    auto contains(long address) -> bool;
    auto create_extent_from(long start) -> Extent*;
    auto take(long offset, long count) -> Extent*;
    auto read(long start, uint8_t* buffer, int offset, int count) -> int;

public:
    int64_t m_offset;
    int64_t m_count;
    std::ifstream *m_stream;
};