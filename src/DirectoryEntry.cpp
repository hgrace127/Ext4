#include "DirectoryEntry.hpp"
#include "byte_buffer2.hpp"

using namespace std;

DirectoryEntry::DirectoryEntry()
{
    m_inode_no = 0;
    m_name = "";
}

DirectoryEntry::DirectoryEntry(uint32_t iNodeNo)
{
    m_inode_no = iNodeNo;
}

DirectoryEntry::DirectoryEntry(uint8_t* b, long bSize ,long offset)
{
    ByteBuffer2 bb (b, bSize, false);

    m_inode_no      = bb.get_uint32_le();
    m_recode_length = bb.get_uint16_le();
    m_name_length   = bb.get_bytes(1)[0];
    m_file_type     = bb.get_bytes(1)[0];
    m_name          = string(reinterpret_cast<const char *>(bb.get_bytes(m_name_length)), m_name_length);
}

auto DirectoryEntry::set_name(string name) -> void
{
    m_name = name;
}