#ifndef UINT_H_
#define UINT_H_

#include <algorithm>
#include <array>
#include <cinttypes>
#include <iomanip>
#include <iostream>
#include <type_traits>



template<int w_,
         typename word_t = typename std::conditional<(w_ <= 8),
                                                     uint8_t, uint64_t>::type,
         int n_ = (w_ <= 8) ? 1 : (w_ + 64 - 1) / 64>
class UInt {
private:
  constexpr static int cmin(int wa, int wb) { return wa < wb ? wa : wb; }
  constexpr static int cmax(int wa, int wb) { return wa > wb ? wa : wb; }

public:
  UInt() {
    for (int i=0; i < n_; i++)
      words_[i] = 0;
  }

  UInt(uint64_t initial) : UInt() {
    words_[0] = initial;
  }

  UInt(std::string initial) {
    // FUTURE: check that literal isn't too big
    int input_bits = 4*(initial.size() - 2);
    int last_start = initial.length();
    for (int word=0; word < n_; word++) {
      if (word * kWordSize > input_bits)
        words_[word] = 0;
      else {
        int word_start = std::max(static_cast<int>(initial.length())
                                  - 16*(word+1), 0);
        int word_len = std::min(16, last_start - word_start);
        last_start = word_start;
        const std::string substr = initial.substr(word_start, word_len);
        uint64_t x = std::stoul(substr, nullptr, 16);
        words_[word] = x;
      }
    }
  }

  template<int other_w>
  explicit UInt(const UInt<other_w> &other) {
    static_assert(other_w <= w_, "Can't copy construct from wider UInt");
    for (int word=0; word < n_; word++) {
      if (word < UInt<other_w>::NW)
        words_[word] = other.words_[word];
      else
        words_[word] = 0;
    }
  }

  template<int out_w>
  UInt<cmax(w_,out_w)> pad() const {
    return UInt<cmax(w_,out_w)>(*this);
  }

  template<int other_w>
  UInt<w_ + other_w> cat(const UInt<other_w> &other) const {
    UInt<w_ + other_w> to_return(other);
    const int offset = other_w % kWordSize;
    for (int i = 0; i < n_; i++) {
      to_return.words_[word_index(other_w) + i] |= words_[i] << offset;
      if ((offset != 0) && (other_w + w_ > kWordSize))
        to_return.words_[word_index(other_w) + i + 1] |= words_[i] >>
          shamt(kWordSize - offset);
    }
    return to_return;
  }

  UInt<w_ + 1> operator+(const UInt<w_> &other) const {
    UInt<w_ + 1> result;
    uint64_t carry = 0;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = words_[i] + other.words_[i] + carry;
      carry = result.words_[i] < words_[i] ? 1 : 0;
    }
    if (kWordSize * n_ == w_)
      result.words_[word_index(w_ + 1)] += carry;
    return result;
  }

  UInt<w_> addw(const UInt<w_> &other) const {
    UInt<w_> result;
    uint64_t carry = 0;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = words_[i] + other.words_[i] + carry;
      carry = result.words_[i] < words_[i] ? 1 : 0;
    }
    result.mask_top_unused();
    return result;
  }

  UInt<w_ + 1> operator-() const {
    // return (~(this->pad<w_+1>())).addw(UInt<w_+1>(1));
    UInt<w_ + 1> result(0);
    uint64_t carry = 1;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = ~words_[i] + carry;
      carry = result.words_[i] < ~words_[i] ? 1 : 0;
    }
    result.mask_top_unused();
    return result;
  }

  UInt<w_ + 1> operator-(const UInt<w_> &other) const {
    UInt<w_ + 1> result;
    uint64_t carry = 0;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = words_[i] - other.words_[i] - carry;
      carry = result.words_[i] > words_[i] ? 1 : 0;
    }
    if (kWordSize * n_ == w_)
      result.words_[word_index(w_ + 1)] -= carry;
    result.mask_top_unused();
    return result;
  }

  UInt<w_ + w_> operator*(const UInt<w_> &other) const {
    UInt<w_ + w_> result(0);
    uint64_t carry = 0;
    for (int i=0; i < n_; i++) {
      carry = 0;
      for (int j=0; j < n_; j++) {
        uint64_t prod_ll = lower(words_[i]) * lower(other.words_[j]);
        uint64_t prod_lu = lower(words_[i]) * upper(other.words_[j]);
        uint64_t prod_ul = upper(words_[i]) * lower(other.words_[j]);
        uint64_t prod_uu = upper(words_[i]) * upper(other.words_[j]);
        uint64_t lower_sum = result.words_[i+j] + carry + lower(prod_ll);
        uint64_t upper_sum = upper(prod_ll) + upper(lower_sum) +
                             lower(prod_lu) + lower(prod_ul);
        result.words_[i+j] = (upper_sum << 32) | lower(lower_sum);
        carry = upper(upper_sum) + upper(prod_lu) + upper(prod_ul) + prod_uu;
      }
      result.words_[i + n_] += carry;
    }
    return result;
  }

  // this / other
  template<int other_w>
  UInt<w_> operator/(const UInt<other_w> &other) const {
    static_assert(w_ <= kWordSize, "Div not supported beyond 64b");
    static_assert(other_w <= kWordSize, "Div not supported beyond 64b");
    return UInt<w_>(as_single_word() / other.as_single_word());
  }

  // this % other
  template<int other_w>
  UInt<cmin(w_, other_w)> operator%(const UInt<other_w> &other) const {
    static_assert(w_ <= kWordSize, "Mod not supported beyond 64b");
    static_assert(other_w <= kWordSize, "Mod not supported beyond 64b");
    return UInt<cmin(w_, other_w)>(as_single_word() % other.as_single_word());
  }

  UInt<w_> operator~() const {
    UInt<w_> result;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = ~words_[i];
    }
    result.mask_top_unused();
    return result;
  }

  UInt<w_> operator&(const UInt<w_> &other) const {
    UInt<w_> result;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = words_[i] & other.words_[i];
    }
    return result;
  }

  UInt<w_> operator|(const UInt<w_> &other) const {
    UInt<w_> result;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = words_[i] | other.words_[i];
    }
    return result;
  }

  UInt<w_> operator^(const UInt<w_> &other) const {
    UInt<w_> result;
    for (int i = 0; i < n_; i++) {
      result.words_[i] = words_[i] ^ other.words_[i];
    }
    return result;
  }

  template<int hi, int lo>
  UInt<hi - lo + 1> bits() const {
    static_assert(hi < w_, "Bit extract hi bigger than width");
    static_assert(hi >= lo, "Bit extract lo > hi");
    static_assert(lo >= 0, "Bit extract lo is negative");
    UInt<hi - lo + 1> result;
    int word_down = word_index(lo);
    int bits_down = lo % kWordSize;
    int top_taken_word = word_index(hi);
    int out_word_width = word_index(hi - lo + kWordSize);
    for (int i=0; i < out_word_width; i++) {
      result.words_[i] = words_[i + word_down] >> bits_down;
      if ((bits_down != 0) && (hi > kWordSize))
        result.words_[i] |= words_[i + word_down + 1] <<
          shamt(kWordSize - bits_down);
    }
    result.mask_top_unused();
    return result;
  }

  template<int n>
  UInt<n> head() const {
    return bits<w_-1, w_-n>();
  }

  template<int n>
  UInt<w_ - n> tail() const {
    return bits<w_-n-1, 0>();
  }

  template<int shamt>
  UInt<w_ + shamt> shl() const {
    return cat(UInt<shamt>(0));
  }

  template<int shamt>
  UInt<w_ - shamt> shr() const {
    return bits<w_-1, shamt>();
  }

  template<int other_w>
  UInt<w_> operator>>(const UInt<other_w> &other) const {
    UInt<w_> result(0);
    uint64_t dshamt = other.as_single_word();
    uint64_t word_down = word_index(dshamt);
    uint64_t bits_down = dshamt % kWordSize;
    for (uint64_t i=word_down; i < n_; i++) {
      result.words_[i - word_down] = words_[i] >> bits_down;
      if ((bits_down != 0) && (i < n_-1))
        result.words_[i - word_down] |= words_[i + 1] <<
          shamt(kWordSize - bits_down);
    }
    return result;
  }

  template<int other_w>
  UInt<w_ + (1<<other_w) - 1> operator<<(const UInt<other_w> &other) const {
    UInt<w_ + (1<<other_w) - 1> result(0);
    uint64_t dshamt = other.as_single_word();
    uint64_t word_up = word_index(dshamt);
    uint64_t bits_up = dshamt % kWordSize;
    for (uint64_t i=0; i < n_; i++) {
      result.words_[i + word_up] |= words_[i] << bits_up;
      if ((bits_up != 0) && (dshamt + w_ > kWordSize))
        result.words_[i + word_up + 1] = words_[i] >>
          shamt(kWordSize - bits_up);
    }
    return result;
  }

  UInt<1> operator<=(const UInt<w_> &other) const {
    for (int i=n_-1; i >= 0; i--) {
      if (words_[i] < other.words_[i]) return UInt<1>(1);
      if (words_[i] > other.words_[i]) return UInt<1>(0);
    }
    return UInt<1>(1);
  }

  UInt<1> operator>=(const UInt<w_> &other) const {
    for (int i=n_-1; i >= 0; i--) {
      if (words_[i] > other.words_[i]) return UInt<1>(1);
      if (words_[i] < other.words_[i]) return UInt<1>(0);
    }
    return UInt<1>(1);
  }

  UInt<1> operator<(const UInt<w_> &other) const {
    return ~(*this >= other);
  }

  UInt<1> operator>(const UInt<w_> &other) const {
    return ~(*this <= other);
  }

  UInt<1> operator==(const UInt<w_> &other) const {
    for (int i = 0; i < n_; i++) {
      if (words_[i] != other.words_[i])
        return UInt<1>(0);
    }
    return UInt<1>(1);
  }

  UInt<1> operator!=(const UInt<w_> &other) const {
    return ~(*this == other);
  }

  operator bool() const {
    static_assert(w_ == 1, "conversion to bool only allowed for width 1");
    return static_cast<bool>(words_[0]);
  }


private:
  // Internal state
  std::array<word_t, n_> words_;

  // Access array word type
  typedef word_t WT;
  // Access array length
  const static int NW = n_;
  // Access array word type bit width
  const static int WW = std::is_same<word_t,uint64_t>::value ? 64 : 8;

  const static int bits_in_top_word_ = w_ % WW == 0 ? WW : w_ % WW;

  // Friend Access
  template<int other_w, typename other_word_t, int other_n>
  friend class UInt;

  template<int other_w>
  friend class SInt;

  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const UInt<w>& ui);

  // Bit Addressing
  const static int kWordSize = 64;

  int static word_index(int bit_index) { return bit_index / kWordSize; }

  uint64_t static upper(uint64_t i) { return i >> 32; }
  uint64_t static lower(uint64_t i) { return i & 0x00000000ffffffff; }

  // Hack to prevent compiler warnings for shift amount being too large
  int static shamt(int s) { return s % kWordSize; }

  // Clean up high bits
  void mask_top_unused() {
    if (bits_in_top_word_ != WW) {
      words_[n_-1] = words_[n_-1] & ((1l << shamt(bits_in_top_word_)) - 1l);
    }
  }

  // Direct access for ops that only need small signals
  uint64_t as_single_word() const {
    static_assert(w_ <= kWordSize, "UInt too big for single uint64_t");
    return words_[0];
  }

  void print_to_stream(std::ostream& os) const {
    os << "0x" << std::hex << std::setfill('0');
    int top_nibble_width = (bits_in_top_word_ + 3) / 4;
    os << std::setw(top_nibble_width);
    uint64_t top_word_mask = bits_in_top_word_ == kWordSize ? -1 :
                               (1l << bits_in_top_word_) - 1;
    os << (static_cast<uint64_t>(words_[n_-1]) & top_word_mask);
    for (int word=n_ - 2; word >= 0; word--) {
     os << std::hex << std::setfill('0') << std::setw(16) << words_[word];
    }
    os << std::dec;
  }
};



template<int w>
std::ostream& operator<<(std::ostream& os, const UInt<w>& ui) {
  ui.print_to_stream(os);
  os << "<U" << w << ">";
  return os;
}

#endif  // UINT_H_
