#include "Ext4Extent.hpp"

Ext4Extent::Ext4Extent(int64_t loff, int64_t poff, int64_t count)
{
    m_l_offset = loff;
    m_p_offset = poff;
    m_count = count;
}