#pragma once

#include <vector>
#include <fstream>

#include "Extent.hpp"

class NodeStream 
{
public:
    NodeStream();
    NodeStream(std::vector<Extent*>* extents, bool compact, long actualLength);
    auto SetLength(long val) -> void;
    auto AllocLength() -> long;

public:
    std::vector<Extent>* m_extents;

    long m_offset = 0;
    long m_actualLength;

    std::fstream* m_nullstream = nullptr;
    // lock

    


private:

};
