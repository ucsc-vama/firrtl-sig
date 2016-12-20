#ifndef UINT_H_
#define UINT_H_

#include <array>
#include <cinttypes>
#include <iomanip>
#include <iostream>


const size_t kWordSize = 64;


template<int w_,
         typename word_t = typename std::conditional<(w_ <= 8), uint8_t, uint64_t>::type,
         int n_ = (w_ <= 8) ? 1 : (w_ + kWordSize - 1) / kWordSize>
class UInt {
public:
  UInt() {
    for (int i=0; i < n_; i++)
      values[i] = 0;
  }

  UInt(uint64_t initial) {
    values[0] = initial;
    for (int i=1; i < n_; i++)
      values[i] = 0;
  }

private:
  std::array<word_t, n_> values;

  // Access array word type
  typedef word_t WT;
  // Access array length
  const static int NW = n_;
  // Access array word type bit width
  const static int WW = std::is_same<word_t,uint64_t>::value ? 64 : 8;

  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const UInt<w>& ui);
};


template<int w>
std::ostream& operator<<(std::ostream& os, const UInt<w>& ui) {
  os << "0x" << std::hex << std::setfill('0');
  int top_bit_width = w % UInt<w>::WW == 0 ? UInt<w>::WW : w % UInt<w>::WW;
  int top_nibble_width = (top_bit_width + 3) / 4;
  os << std::setw(top_nibble_width);
  os << static_cast<uint64_t>(ui.values[UInt<w>::NW-1]);
  for (int word=UInt<w>::NW - 2; word >= 0; word--) {
   os << std::hex << std::setfill('0') << std::setw(16) << ui.values[word];
  }
  os << std::dec << "<U" << w << ">";
  return os;
}

#endif  // UINT_H_
