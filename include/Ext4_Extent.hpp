#pragma once

class Ext4_Extent{
public:
    Ext4_Extent();
    Ext4_Extent(long loff, long poff, long count);
public:
    long m_LOffset;
    long m_POffset;
    long m_count;
};