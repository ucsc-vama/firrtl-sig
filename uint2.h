#ifndef UINT_H_
#define UINT_H_

#include <algorithm>
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

  UInt(std::string initial) {
    // FUTURE: check that literal isn't too big
    int input_bits = 4*(initial.size() - 2);
    int last_start = initial.length();
    for (int word=0; word < n_; word++) {
      if (word * kWordSize > input_bits)
        values[word] = 0;
      else {
        int word_start = std::max(static_cast<int>(initial.length())
                                  - 16*(word+1), 0);
        int word_len = std::min(16, last_start - word_start);
        last_start = word_start;
        const std::string substr = initial.substr(word_start, word_len);
        uint64_t x = std::stoul(substr, nullptr, 16);
        values[word] = x;
      }
    }
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
