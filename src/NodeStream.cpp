#include <vector>

#include "NodeStream.hpp"

using namespace std;

NodeStream::NodeStream()
{
    m_extents = new vector<Extent*>();
}

NodeStream::NodeStream(vector<Extent*>* extents, bool compact = false, long actualLength = 0){
    if (compact && extents->size() > 0)
    {
        vector<Extent*>* compacted = new vector<Extent*>;
        for(int i=0; i < extents->size(); i++){
            compacted->push_back(extents->at(i));
        }

        for(int i = 1; i < extents->size(); i++){
            Extent last = *compacted->back();

            if (last.m_offset + last.m_count == extents->at(i)->m_offset)
                last.m_count += extents->at(i)->m_count;
            else 
                compacted->push_back(extents->at(i));
        }

        m_extents = compacted;
    }
    else 
    {
        m_extents = extents;
    }

    SetLength(actualLength > 0 ? actualLength : AllocLength());
}

auto NodeStream::SetLength(long val) -> void
{
    m_actualLength = val;
}

auto NodeStream::AllocLength() -> long
{
    vector<Extent>::iterator it;
    long rst = 0;
    for(int i=0; i < m_extents->size();i++){
        rst += m_extents->at(i)->m_count;
    }

    return rst;
}