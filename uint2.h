#ifndef UINT_H_
#define UINT_H_

#include <algorithm>
#include <array>
#include <cinttypes>
#include <iomanip>
#include <iostream>


const int kWordSize = 64;

constexpr int cmax(int wa, int wb) { return wa > wb ? wa : wb; }

constexpr int words_needed(int bit_width) {
  return (bit_width + kWordSize - 1) / kWordSize;
}

int word_index(int bit_index) { return bit_index / kWordSize; }

template<int w_,
         typename word_t = typename std::conditional<(w_ <= 8), uint8_t, uint64_t>::type,
         int n_ = (w_ <= 8) ? 1 : words_needed(w_)>
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

  template<int other_w>
  explicit UInt(const UInt<other_w> &other) {
    // FUTURE: check that other_w <= w_
    for (int word=0; word < n_; word++) {
      if (word < UInt<other_w>::NW)
        values[word] = other.values[word];
      else
        values[word] = 0;
    }
  }

  template<int out_w>
  UInt<cmax(w_,out_w)> widen() {
    return UInt<cmax(w_,out_w)>(*this);
  }

  template<int other_w>
  UInt<w_ + other_w> cat(const UInt<other_w> &other) {
    UInt<w_ + other_w> to_return(other);
    const int offset = other_w % kWordSize;
    for (int i = 0; i < n_; i++) {
      to_return.values[word_index(other_w) + i] |= values[i] << offset;
      if (offset != 0)
        to_return.values[word_index(other_w) + i + 1] |= values[i] >>
          std::min(kWordSize-1, (kWordSize - offset));
    }
    return to_return;
  }

  UInt<w_ + 1> operator+(const UInt<w_> &other) {
    UInt<w_ + 1> result(other);
    uint64_t carry = 0;
    for (int i = 0; i < n_; i++) {
      result.values[i] = values[i] + other.values[i] + carry;
      carry = result.values[i] < values[i] ? 1 : 0;
    }
    if (kWordSize * n_ == w_)
      result.values[word_index(w_ + 1)] += carry;
    return result;
  }


private:
  std::array<word_t, n_> values;

  // Access array word type
  typedef word_t WT;
  // Access array length
  const static int NW = n_;
  // Access array word type bit width
  const static int WW = std::is_same<word_t,uint64_t>::value ? 64 : 8;

  const static int bits_in_top_word = w_ % WW == 0 ? WW : w_ % WW;

  template<int other_w, typename other_word_t, int other_n>
  friend class UInt;

  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const UInt<w>& ui);
};


template<int w>
std::ostream& operator<<(std::ostream& os, const UInt<w>& ui) {
  os << "0x" << std::hex << std::setfill('0');
  int top_nibble_width = (ui.bits_in_top_word + 3) / 4;
  os << std::setw(top_nibble_width);
  os << static_cast<uint64_t>(ui.values[UInt<w>::NW-1]);
  for (int word=UInt<w>::NW - 2; word >= 0; word--) {
   os << std::hex << std::setfill('0') << std::setw(16) << ui.values[word];
  }
  os << std::dec << "<U" << w << ">";
  return os;
}

#endif  // UINT_H_
