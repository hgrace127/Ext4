#pragma once

#include <stdint.h>

class Ext4Extent{
public:
    Ext4Extent(int64_t loff, int64_t poff, int64_t count);
public:
    int64_t m_l_offset;
    int64_t m_p_offset;
    int64_t m_count;
};