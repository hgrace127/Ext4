#include "byte_buffer2.hpp"
#include "endian_swap.hpp"

#include <iostream>
#include <utility>
#include <cctype>
#include <cstring>
#include <codecvt>
#include <locale>
#include <sstream>
#include <algorithm>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//
// ByteBuffer2 is for shallow version of buffer
//
////////////////////////////////////////////////////////////////////////////////
ByteBuffer2::ByteBuffer2(uint8_t* data, int offset, int count, bool owner)
{
  m_data   = data;
  m_offset = offset;
  m_begin  = offset;
  m_count  = count;
  m_limit  = m_begin + count;
  m_owner  = owner;
}

ByteBuffer2::ByteBuffer2(uint8_t* data, int count, bool owner)
  : ByteBuffer2(data, 0, count, owner)
{}

ByteBuffer2::ByteBuffer2()
  : ByteBuffer2(nullptr, 0, 0)
{}

ByteBuffer2::ByteBuffer2(string && src)
{
  auto len = src.length();

  if (len > 0) 
  {
    m_data = new uint8_t[len];
    for (int i=0; i<len; i++) m_data[i] = src[i];

    m_begin  = 0;
    m_offset = 0;
    m_count  = int(len);
    m_limit  = int(m_begin + len);
    m_owner  = true;
  }
}

ByteBuffer2::ByteBuffer2(initializer_list<uint8_t> l)
{
  reset(l);
}

ByteBuffer2::ByteBuffer2(ByteBuffer2&& rhs)
{
  m_data   = rhs.m_data;
  m_offset = rhs.m_offset;
  m_begin  = rhs.m_begin;
  m_count  = rhs.m_count;
  m_limit  = rhs.m_limit;
  m_owner  = rhs.m_owner;
  
  rhs.m_data  = nullptr;
  rhs.m_owner = false;
}

ByteBuffer2::~ByteBuffer2()
{
  destroy();
}

auto ByteBuffer2::destroy() -> void
{
  if (m_owner)
  {
    delete [] m_data;
    m_data = nullptr;
  }
}

//
// Note:
//   This function is implemented naively, on purpose,
//   because it'll be used rarely.
//
auto ByteBuffer2::append(uint8_t* buffer, int offset, int count) -> int
{
  if (!m_owner || offset < 0 || count <= 0)
    return -1;
  
  if (buffer == nullptr)
    return -1;
  
  auto new_size = m_count + count;
  auto new_data = new uint8_t[new_size];
  
  memcpy(new_data, m_data, m_count);
  memcpy(new_data + m_count, buffer, count);
  
  m_data  = new_data;
  m_count = new_size;
  m_limit = new_size;
  
  return new_size;
}

auto ByteBuffer2::operator[](uint32_t index) -> uint8_t&
{
  check_offset(index);
  return m_data[m_begin + index];
}

auto ByteBuffer2::operator[](uint32_t index) const -> uint8_t
{
  check_offset(index);

  return m_data[m_begin + index];
}

auto ByteBuffer2::get_int8(int at) const -> int8_t
{
  check_offset(1);

  auto here = advance(at, 1);
  return (int8_t)m_data[here];
}

auto ByteBuffer2::get_uint8(int at) const -> uint8_t
{
  check_offset(1);

  auto here = advance(at, 1);
  return m_data[here];
}

auto ByteBuffer2::get_int16_be(int at) const -> int16_t
{
  check_offset(2);

  auto here = advance(at, 2);
  auto res  = *(int16_t *)(uint8_t *)&m_data[here];

  return endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(res);
}

auto ByteBuffer2::get_int16_le(int at) const -> int16_t
{
  check_offset(2);

  auto here = advance(at, 2);
  auto res  = *(int16_t *)(uint8_t *)&m_data[here];

  return res;
}

auto ByteBuffer2::get_uint16_be(int at) const -> uint16_t
{
  check_offset(2);

  auto here = advance(at, 2);
  auto res  = *(uint16_t *)(uint8_t *)&m_data[here];

  return endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(res);
}

auto ByteBuffer2::get_uint16_le(int at) const -> uint16_t
{
  check_offset(2);

  auto here = advance(at, 2);
  auto res  = *(uint16_t *)(uint8_t *)&m_data[here];

  return res;
}

auto ByteBuffer2::get_int24_be(int at) const -> int32_t
{
  check_offset(3);

  auto here = advance(at, 3);
  auto res  = (int32_t)leading_byte(m_data[here]);

  for (int i=here; i<here+3; i++)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int24_le(int at) const -> int32_t
{
  check_offset(3);

  auto here = advance(at, 3);
  
  auto first = m_data[here+2];
  auto l = (int32_t) leading_byte(first);
  auto r = l << 8 | first; r <<= 8;
  r |= m_data[here + 1]; r <<= 8;
  r |= m_data[here + 0];
  
  return int32_t(r);
}

auto ByteBuffer2::get_uint24_be(int at) const -> uint32_t
{
  check_offset(3);

  auto here = advance(at, 3);
  auto res  = (uint32_t)0;

  for (int i=here; i<here+3; i++)
    res = (res << 8) + m_data[i];

  return res;
}

auto ByteBuffer2::get_uint24_le(int at) const -> uint32_t
{
  check_offset(3);
  auto here = advance(at, 3);
  auto res  = uint32_t(m_data[here+2]); res <<= 8;
  res |= m_data[here + 1]; res <<= 8;
  res |= m_data[here];
  
  return res;
}

auto ByteBuffer2::get_int32_be(int at) const -> int32_t
{
  check_offset(4);

  auto here = advance(at, 4);
  auto res  = *(int32_t*)(uint8_t*)&m_data[here];

  return endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(res);
}

auto ByteBuffer2::get_int32_le(int at) const -> int32_t
{
  check_offset(4);

  auto here = advance(at, 4);
  auto res  = *(int32_t *)(uint8_t *)&m_data[here];

  return res;
}

auto ByteBuffer2::get_uint32_be(int at) const -> uint32_t
{
  check_offset(4);

  auto here = advance(at, 4);
  auto res  = *(uint32_t *)(uint8_t *)&m_data[here];

  return endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(res);
}

auto ByteBuffer2::get_uint32_le(int at) const -> uint32_t
{
  check_offset(4);

  auto here = advance(at, 4);
  auto res  = *(uint32_t *)(uint8_t *)&m_data[here];

  return res;
}

auto ByteBuffer2::get_int40_be(int at) const -> int64_t
{
  check_offset(5);

  auto here = advance(at, 5);
  auto sign = (int64_t)leading_byte(m_data[here]);
  auto res  = (sign << 16) | (sign << 8) | sign;

  for (int i=here; i<here+5; i++)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int40_le(int at)  const -> int64_t
{
  check_offset(5);

  auto here = advance(at, 5);
  auto sign = (int64_t)leading_byte(m_data[here+4]);
  auto res  = (sign << 16) | (sign << 8) | sign;

  for (int i=here+4; i>=here; i--)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_uint40_be(int at) const -> uint64_t
{
  check_offset(5);

  auto here = advance(at, 5);
  auto res  = (uint64_t)0;

  for (int i=here; i<here+5; i++)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_uint40_le(int at) const -> uint64_t
{
  check_offset(5);

  auto here = advance(at, 5);
  auto res  = (uint64_t)0;
  for (int i=here+4; i>=here; i--)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int48_be(int at) const -> int64_t
{
  check_offset(6);

  auto here = advance(at, 6);
  auto sign = (int64_t)leading_byte(m_data[here]);
  auto res  = (sign << 8) | sign;

  for (int i=here; i<here+6; i++)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int48_le(int at) const -> int64_t
{
  check_offset(6);

  auto here = advance(at, 6);
  auto sign = (int64_t)leading_byte(m_data[m_offset+5]);
  auto res  = (sign << 8) | sign;

  for (int i=here+5; i>=here; i--)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_uint48_be(int at) const -> uint64_t
{
  check_offset(6);

  auto here = advance(at, 6);
  auto res  = (uint64_t)0;

  for (int i=here; i<here+6; i++)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_uint48_le(int at) const -> uint64_t
{
  check_offset(6);

  auto here = advance(at, 6);
  auto res  = (uint64_t)0;

  for (int i=here+5; i>=here; i--)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int56_be(int at) const -> int64_t
{
  check_offset(7);

  auto here = advance(at, 7);
  auto res  = (int64_t)leading_byte(m_data[m_offset]);

  for (int i=here; i<here+7; i++)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int56_le(int at) const -> int64_t
{
  check_offset(7);

  auto here = advance(at, 7);
  auto res  = (int64_t)leading_byte(m_data[m_offset+6]);

  for (int i=here+6; i>=here; i--)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_uint56_be(int at) const -> uint64_t
{
  check_offset(7);

  auto here = advance(at, 7);
  auto res  = (uint64_t)0;

  for (int i=here; i<here+7; i++)
    res = (res << 8) + m_data[i]; 

  m_offset += 7;

  return res;
}

auto ByteBuffer2::get_uint56_le(int at) const -> uint64_t
{
  check_offset(7);

  auto here = advance(at, 7);
  auto res  = (uint64_t)0;

  for (int i=here+6; i>=here; i--)
    res = (res << 8) + m_data[i]; 

  return res;
}

auto ByteBuffer2::get_int64_be(int at) const -> int64_t
{
  check_offset(8);

  auto here = advance(at, 8);
  auto res  = *(int64_t *)(uint8_t *)&m_data[here];

  return endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(res);
}

auto ByteBuffer2::get_int64_le(int at) const -> int64_t
{
  check_offset(8);

  auto here = advance(at, 8);
  auto res  = *(int64_t *)(uint8_t *)&m_data[here];

  return res;
}

auto ByteBuffer2::get_uint64_be(int at) const -> uint64_t
{
  check_offset(8);

  auto here = advance(at, 8);
  auto res  = *(uint64_t *)(uint8_t *)&m_data[here];

  return endian_swap_bytes<HOST_ENDIAN_ORDER, BIG_ENDIAN_ORDER>(res);
}

auto ByteBuffer2::get_uint64_le(int at) const -> uint64_t
{
  check_offset(8);

  auto here = advance(at, 8);
  auto res  = *(uint64_t *)(uint8_t *)&m_data[here];

  return res;
}

auto ByteBuffer2::get_double(int at) const -> double
{
  auto res = get_int64_be(at);

  return *(double *)&res;
}

auto ByteBuffer2::get_bytes(int size, int at) const -> uint8_t*
{
  check_offset(size);
  
  auto here = advance(at, size);
  auto res  = &m_data[here];

  return res;
}

auto ByteBuffer2::get_hex_string(int size, int at) -> const string
{
  constexpr char hmap[] =
    { '0', '1', '2', '3', '4', '5', '6', '7',
      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

  auto here = advance(at, size);
  string res(size * 2, ' ');

  for (int i=0; i<size; i++)
  {
    res[2*i + 0] = hmap[(m_data[here + i] & 0xf0) >> 4];
    res[2*i + 1] = hmap[(m_data[here + i] & 0x0f)];
  }

  return res;
}

auto ByteBuffer2::get_varint() const -> int64_t
{
  auto [res, size] = get_varint_with_size();
  return res;
}

auto ByteBuffer2::get_varint2() const -> pair<int64_t, int>
{
  return get_varint_with_size();
}

auto ByteBuffer2::get_varint_with_size() const -> pair<int64_t, int>
{
  auto complete = false;
  auto value_size = 0;
  auto value = 0L;

  while (value_size < 9 && !complete && m_offset + value_size < m_limit)
  {
    auto val = int64_t(m_data[m_offset + value_size]);
    if ((val & 0b10000000) == 0b10000000 && value_size < 8)
    {
      value = value << 7 | (val & 0b01111111);
    } 
    else if ((val & 0b10000000) == 0b10000000 && value_size == 8)
    {
      value = value << 8 | val;
      complete = true;
    } 
    else 
    {
      value = value << 7 | (val & 0b01111111);
      complete = true;
    }   

    value_size += 1;
  }

  if (!complete)
    throw logic_error("invalid varint");

  m_offset += value_size;

  return make_pair(value, value_size);
}

auto ByteBuffer2::has_remaining() const -> bool
{
  return m_offset < m_limit;
}

auto ByteBuffer2::remained_size() const -> int
{
  return m_limit - m_offset;
}

auto ByteBuffer2::advance(int count) -> ByteBuffer2&
{
  m_offset += count;

  return *this;
}

auto ByteBuffer2::skip(int count) -> ByteBuffer2&
{
  m_offset += count;

  return *this;
}

auto ByteBuffer2::unget(int count) -> ByteBuffer2&
{
  m_offset -= count;

  return *this;
}
  
auto ByteBuffer2::take(int amount) const -> ByteBuffer2
{
  ByteBuffer2 subrange(m_data, m_offset, amount);
  m_offset += amount;

  return subrange;
}

auto ByteBuffer2::slice(int from, int count) -> ByteBuffer2
{
  return ByteBuffer2(m_data, from, count);
}
  
auto ByteBuffer2::copy_slice(int from, int count) -> ByteBuffer2
{
  auto data = new uint8_t[count];
  memcpy(data, m_data + from, count);
  
  return ByteBuffer2(data, count, true);
}

auto ByteBuffer2::compare_range(int from, int count, uint8_t value) -> bool
{
  if (from + count >= m_limit) return false;

  for (int i=from; i<from+count; i++)
    if (m_data[i] != value) 
      return false;

  return true;
}

auto ByteBuffer2::first() const -> uint8_t
{
  check_offset(1);

  return m_data[0];
}

auto ByteBuffer2::first(int amount) const -> ByteBuffer2
{
  check_offset(amount);

  return ByteBuffer2(m_data, m_begin, amount);
}

auto ByteBuffer2::last() const -> uint8_t
{
  check_offset(1);

  return m_data[m_limit - 1];
}

auto ByteBuffer2::last(int amount) const -> ByteBuffer2
{
  ByteBuffer2 subrange(m_data, m_limit - amount, amount);

  return subrange;
}

auto ByteBuffer2::starts_with(string const& str) const -> bool
{
  if ((int)str.length() > m_count)
    throw out_of_range("starts_with: array out of index");
    
  auto len = str.length();
  string s((char*)&m_data[0], len);

  return str == s;
}

auto ByteBuffer2::reset() -> ByteBuffer2&
{
  m_offset = m_begin;

  return *this;
}
 
auto ByteBuffer2::reset(initializer_list<uint8_t> l) -> void
{
  m_data = new uint8_t[l.size()];
  int i = 0;
  for (auto it = l.begin(); it != l.end(); ++it)
    m_data[i++] = *it;

  m_offset = 0;
  m_begin  = 0;
  m_count  = (int)l.size();
  m_limit  = m_count;
  m_owner  = true;
}

/*
auto ByteBuffer2::reset(initializer_list<uint8_t> l) -> void
{
  m_data   = (uint8_t *)&*l.begin();
  m_offset = 0;
  m_begin  = 0;
  m_count  = (int)l.size();
  m_limit  = m_count;
  m_owner  = false;
}
*/

auto ByteBuffer2::get_ascii(int size) const -> string
{
  if (m_offset + size > m_limit)
    throw out_of_range("get_ascii: out of range");
  
  string res((char*)&m_data[m_offset], size);

  m_offset += size;

  return res;
}

auto ByteBuffer2::get_ascii() const -> string
{
  auto offset = m_offset;

  for (; offset < m_limit && m_data[offset] != 0; ++offset)
    ;

  auto size = offset - m_offset;

  string res((char*)&m_data[m_offset], size);

  m_offset += size + 1;

  return res;
}

auto ByteBuffer2::get_unicode16_le(int size) const -> string
{
  throw logic_error("wating for standardization");
  /*
  if (m_offset + size*2 > m_limit)
    throw out_of_range("get_unicode16_le(sz): out of range");
  using converter = wstring_convert<
    codecvt_utf8_utf16<char16_t, 0x10ffff, (codecvt_mode)1>,
    char16_t>;
  converter c;
  auto beg = (char16_t *)&m_data[m_offset];
  auto res = c.to_bytes(beg, beg + size);
  m_offset += size*2;
  return res;
  */
}

auto ByteBuffer2::from_hexcode(string const& s, bool is_be) -> ByteBuffer2
{
  auto all_hex = all_of(s.cbegin(), s.cend(), 
      [](int c) { return isxdigit(c) != 0; });

  if (s.length() % 2 == 0 && all_hex)
  {
    auto size = int(s.length() / 2);
    auto data = new uint8_t[size];

    for (int i=0; i<size; ++i)
    {
      auto in = is_be 
        ? s.substr((size - (i + 1)) * 2, 2)
        : s.substr(i * 2, 2);

      istringstream strm(in);
      uint16_t val; strm >> hex >> val;
      data[i] = (uint8_t)val;
    }

    ByteBuffer2 res(data, 0, size, true);
    return res;
  }
  
  return ByteBuffer2();
}

auto ByteBuffer2::to_s(int from, int to) const -> std::string
{
  if (from == -1) from = m_offset;
  if (to == -1) to = m_limit;

  return string((char*)&m_data[from], to - from);
}

auto ByteBuffer2::check_offset(int count) const -> void
{
  if (m_offset + count > m_limit)
    throw out_of_range("check_offset: array out of index");
}

auto ByteBuffer2::leading_byte(uint8_t b) const -> uint8_t 
{
  return (b & 0x80) == 0 ? 0 : 0xff;
}

auto ByteBuffer2::advance(int at, int dist) const -> int
{
  auto here = (at == -1) ? m_offset : m_begin + at;
  if (at == -1)
    m_offset += dist;

  return here;
}

void ByteBuffer2::debug_it() const
{
  cout << "=================="   << endl;
  cout << "offset: " << m_offset << endl;
  cout << "begin:  " << m_begin  << endl;
  cout << "count:  " << m_count  << endl;
  cout << "limit:  " << m_limit  << endl;
  cout << "=================="   << endl;
}

////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////