#pragma once

#include <stdint.h>
#include <string>

class DirectoryEntry
{
public:
    DirectoryEntry();
    DirectoryEntry(uint32_t iNodeNo);
    DirectoryEntry(uint8_t* b, long bSize ,long offset);
    auto SetName(std::string name) -> void;

private:


public:
    long m_Address;
    uint32_t m_iNodeNo;
    uint32_t m_recodeLength;
    int8_t m_nameLength;
    int8_t m_fileType;
    std::string m_name;
private:
};