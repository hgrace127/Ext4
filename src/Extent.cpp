#include "Extent.hpp"


using namespace std;

Extent::Extent(fstream* stream, long offset = -1L, long count = -1L)
{
    m_stream = stream;
    m_offset = offset;
    m_count = count;
}

auto Extent::Take(long offset, long count = 0) -> Extent*
{
    if (offset < 0 || offset >= m_count)
        return nullptr;

    long  limit = m_count - offset;

    if (limit <= 0)
        return nullptr;

    if (count > 0 && count < limit)
        limit = count;

    return new Extent(m_stream, m_offset + offset, limit);
}

auto Extent::CreateExtentFrom(long start) -> Extent*
{
    return m_offset > start ? new Extent(m_stream, start, m_offset - start) : nullptr;
}

auto Extent::Read(long start, uint8_t* buffer, int offset, int count) -> int 
{
    if (!m_stream->is_open() || !m_stream->good())
        return -1;

    long here = m_offset + start;
    m_stream->seekg(here, m_stream->beg);
    m_stream->read((char*)buffer, count);
    
    return count;
}

auto Extent::Contains(long address) -> bool
{
    return m_offset >= 0 && address >= m_offset && address < m_offset + m_count;
}