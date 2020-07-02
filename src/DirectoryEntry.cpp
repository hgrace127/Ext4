#include "DirectoryEntry.hpp"
#include "byte_buffer2.hpp"

using namespace std;

DirectoryEntry::DirectoryEntry()
{
    m_iNodeNo = 0;
    m_name = "";
}

DirectoryEntry::DirectoryEntry(uint32_t iNodeNo)
{
    m_iNodeNo = iNodeNo;
}

DirectoryEntry::DirectoryEntry(uint8_t* b, long bSize ,long offset)
{
    ByteBuffer2 bb (b, bSize, false);

    m_iNodeNo = bb.get_uint32_le();
    m_recodeLength = bb.get_uint32_le();
    m_nameLength = bb.get_bytes(1)[0];
    m_fileType = bb.get_bytes(1)[0];
    m_name = string(reinterpret_cast<const char *>(bb.get_bytes(m_nameLength)), m_nameLength);
}

auto DirectoryEntry::SetName(string name) -> void
{
    m_name = name;
}