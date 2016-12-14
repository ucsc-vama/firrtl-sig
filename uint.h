#ifndef UINT_H_
#define UINT_H_

#include <algorithm>
#include <array>
#include <cinttypes>
#include <iomanip>
#include <iostream>
#include <string>


const size_t kWordSize = 64;
constexpr static size_t words_needed(int desired_width) {
  return (desired_width + kWordSize - 1) / kWordSize;
}

template<int w, bool wide = (w > kWordSize)>
class UInt;


template<int w_>
class UInt<w_, /*wide=*/false> {
public:
  UInt(uint64_t initial) : value(initial) {}

  template<int out_w>
  UInt<out_w> widen(int x=out_w) {
    return UInt<out_w>(value);
  }

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


template<int w_>
class UInt<w_, /*wide=*/true> {
public:
  UInt(uint64_t initial) {
    values[0] = initial;
    for (int word=1; word < words_needed(w_); word++) {
      values[word] = 0;
    }
  }

  UInt(std::string initial) {
    // FUTURE: check that literal isn't too big
    int top_bit_width = w_ % kWordSize == 0 ? kWordSize : w_ % kWordSize;
    int top_nibble_width = (top_bit_width + 3) / 4;
    int input_bits = 4*(initial.size() - 2);
    int last_start = initial.length();
    for (int word=0; word < words_needed(w_); word++) {
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
  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const UInt<w,true>& ui);
  std::array<uint64_t, words_needed(w_)> values;
};

template<int w>
std::ostream& operator<<(std::ostream& os, const UInt<w,true>& ui) {
  os << "0x" << std::hex << std::setfill('0');
  int top_bit_width = w % kWordSize == 0 ? kWordSize : w % kWordSize;
  int top_nibble_width = (top_bit_width + 3) / 4;
  const int total_words = words_needed(w);
  os << std::setw(top_nibble_width) << ui.values[total_words-1];
  for (int word=total_words - 2; word >= 0; word--) {
   os << std::hex << std::setfill('0') << std::setw(16) << ui.values[word];
  }
  os << std::dec << "<U" << w << ">";
  return os;
}

#endif  // UINT_H_
