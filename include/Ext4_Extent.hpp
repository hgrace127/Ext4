#pragma once

#include <stdint.h>

class Ext4_Extent{
public:
    Ext4_Extent(int64_t loff, int64_t poff, int64_t count);
public:
    int64_t m_LOffset;
    int64_t m_POffset;
    int64_t m_count;
};