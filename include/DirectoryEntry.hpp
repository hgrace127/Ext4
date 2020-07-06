#pragma once

#include <stdint.h>
#include <string>

class DirectoryEntry
{
public:
    DirectoryEntry();
    DirectoryEntry(uint32_t iNodeNo);
    DirectoryEntry(uint8_t* b, long bSize ,long offset);
    auto set_name(std::string name) -> void;

public:
    int64_t m_address;
    uint32_t m_inode_no;
    uint32_t m_recode_length;
    int8_t m_name_length;
    int8_t m_file_type;
    std::string m_name;
};