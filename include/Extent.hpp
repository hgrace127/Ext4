#pragma once

#include <fstream>
#include <stdint.h>

class Extent
{
public:
    Extent(std::fstream* stream, long offset, long count);
    auto Take(long offset, long count) -> Extent*;
    auto CreateExtentFrom(long start) -> Extent*;
    auto Read(long start, uint8_t* buffer, int offset, int count) -> int;
    auto Contains(long address) -> bool;

public:
    int64_t m_offset;
    int64_t m_count;
    std::fstream *m_stream;
};