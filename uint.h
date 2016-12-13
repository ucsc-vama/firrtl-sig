#ifndef UINT_H_
#define UINT_H_

#include <array>
#include <cinttypes>
#include <iomanip>
#include <iostream>

const size_t kWordSize = 64;

template<int w, bool wide = (w > kWordSize)>
class UInt;


template<int w_>
class UInt<w_, /*wide=*/false> {
public:
  UInt(uint64_t initial) : value(initial) {}

private:
  typedef typename std::conditional<(w_ <= 8), uint8_t, uint64_t>::type data_t;
  data_t value;
  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const UInt<w,false>& ui);
};

template<int w>
std::ostream& operator<<(std::ostream& os, const UInt<w,false>& ui) {
  int str_width = (w + 3) / 4;
  os << "0x" << std::hex << std::setfill('0') << std::setw(str_width) << ui.value;
  os << std::dec << "<U" << w << ">";
  return os;
}


template<int w>
class UInt<w, /*wide=*/true> {
public:
  UInt(uint64_t initial) {
    values[0] = initial;
  }

private:
  constexpr static size_t words_needed(int desired_width) {
    return (desired_width + kWordSize - 1) / kWordSize;
  }
  std::array<uint64_t, words_needed(w)> values;
};

template<int w>
std::ostream& operator<<(std::ostream& os, const UInt<w,true>& ui) {
  os << "0x" << std::hex << ui.values[0] << "<U" << w << ">";
  return os;
}

#endif  // UINT_H_
