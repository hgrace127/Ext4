#include "Ext4_Extent.hpp"

Ext4_Extent::Ext4_Extent(int64_t loff, int64_t poff, int64_t count)
{
    m_LOffset = loff;
    m_POffset = poff;
    m_count = count;
}