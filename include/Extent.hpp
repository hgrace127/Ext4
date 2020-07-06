#pragma once

#include <fstream>
#include <stdint.h>

class Extent
{
public:
    Extent(std::ifstream* stream, long offset, long count);
    auto take(long offset, long count) -> Extent*;
    auto create_extent_from(long start) -> Extent*;
    auto read(long start, uint8_t* buffer, int offset, int count) -> int;
    auto contains(long address) -> bool;

public:
    int64_t m_offset;
    int64_t m_count;
    std::ifstream *m_stream;
};