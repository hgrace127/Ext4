#include <vector>

#include "NodeStream.hpp"

using namespace std;

NodeStream::NodeStream() {}

NodeStream::NodeStream(vector<Extent*> extents, bool compact = false, long actualLength = 0){
    if (compact && extents.size() > 0)
    {
        vector<Extent*> compacted;
        for(int i=0; i < extents.size(); i++){
            compacted.push_back(extents.at(i));
        }

        for(int i = 1; i < extents.size(); i++){
            Extent* last = compacted.back();

            if (last->m_offset + last->m_count == extents[i]->m_offset)
                last->m_count += extents[i]->m_count;
            else 
                compacted.push_back(extents[i]);
        }

        m_extents = compacted;
    }
    else 
    {
        m_extents = extents;
    }

    set_length(actualLength > 0 ? actualLength : alloc_length());
}

auto NodeStream::set_length(long val) -> void
{
    m_actualLength = val;
}

auto NodeStream::alloc_length() -> long
{
    vector<Extent>::iterator it;
    long rst = 0;
    for(int i=0; i < m_extents.size();i++){
        rst += m_extents[i]->m_count;
    }

    return rst;
}