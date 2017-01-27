#ifndef SINT_H_
#define SINT_H_

#include <cinttypes>

#include "uint.h"


template<int w_>
class SInt {
  private: // copied from uint.h
  constexpr static int cmin(int wa, int wb) { return wa < wb ? wa : wb; }
  constexpr static int cmax(int wa, int wb) { return wa > wb ? wa : wb; }

public:
  SInt() : ui(0) {}

  SInt(int64_t i) : ui(i) {
    sign_extend(kWordSize - 1);
  }

  SInt(std::string initial) : ui(initial) {
    sign_extend();
  }

  template<int other_w>
  explicit SInt(const SInt<other_w> &other) {
    static_assert(other_w <= w_, "Can't copy construct from wider SInt");
    ui = UInt<w_>(other.ui);
    sign_extend(other_w - 1);
  }

  SInt(const UInt<w_> &other) : ui(other) {}

  template<int out_w>
  SInt<cmax(w_,out_w)> pad() const {
    return SInt<cmax(w_,out_w)>(*this);
  }

  template<int other_w>
  SInt<w_ + other_w> cat(const SInt<other_w> &other) const {
    SInt<w_ + other_w> result(ui.cat(other.ui));
    result.sign_extend();
    return result;
  }

  SInt<w_ + 1> operator+(const SInt<w_> &other) const {
    SInt<w_+1> result(ui.template core_add_sub<w_+1, false>(other.ui));
    if ((w_ % kWordSize == 0) &&
        (result.ui.words_[ui.word_index(w_-1)] < ui.words_[ui.word_index(w_-1)])) {
      result.ui.words_[ui.word_index(w_)] = -1;
    }
    return result;
  }

  SInt<w_> addw(const SInt<w_> &other) const {
    SInt<w_> result(ui.template core_add_sub<w_, false>(other.ui));
    return result;
  }

  SInt<w_ + 1> operator-() const {
    SInt<w_ + 1> result = SInt<w_>(0) - *this;
    return result;
  }

  SInt<w_ + 1> operator-(const SInt<w_> &other) const {
    SInt<w_ + 1> result(ui.template core_add_sub<w_+1, true>(other.ui));
    if ((w_ % kWordSize == 0) &&
        (result.ui.words_[ui.word_index(w_-1)] > ui.words_[ui.word_index(w_-1)])) {
      result.ui.words_[ui.word_index(w_)] = -1;
    }
    return result;
  }

  SInt<w_ + w_> operator*(const SInt<w_> &other) const {
    SInt<4*w_> result(pad<w_ + w_>().ui * other.pad<w_ + w_>().ui);
    return result.template tail<w_ + w_>();
  }

  template<int other_w>
  SInt<w_> operator/(const SInt<other_w> &other) const {
    static_assert(w_ <= kWordSize, "Div not supported beyond 64b");
    static_assert(other_w <= kWordSize, "Div not supported beyond 64b");
    return SInt<w_>(as_single_word() / other.as_single_word());
  }

  template<int other_w>
  SInt<cmin(w_, other_w)> operator%(const SInt<other_w> &other) const {
    return SInt<cmin(w_, other_w)>(ui % other.ui);
  }

  SInt<w_> operator~() const {
    return SInt<w_>(ui.core_negate());
  }

  SInt<w_> operator&(const SInt<w_> &other) const {
    return SInt<w_>(ui & other.ui);
  }

  SInt<w_> operator|(const SInt<w_> &other) const {
    return SInt<w_>(ui | other.ui);
  }

  SInt<w_> operator^(const SInt<w_> &other) const {
    return SInt<w_>(ui ^ other.ui);
  }

  template<int hi, int lo>
  SInt<hi - lo + 1> bits() const {
    SInt<hi - lo + 1> result(ui.template core_bits<hi,lo>());
    result.sign_extend(hi - lo);
    return result;
  }

  template<int n>
  SInt<n> head() const {
    return bits<w_-1, w_-n>();
  }

  template<int n>
  SInt<w_ - n> tail() const {
    return bits<w_-n-1, 0>();
  }

  template<int shamt>
  SInt<w_ + shamt> shl() const {
    return cat(SInt<shamt>(0));
  }

  template<int shamt>
  SInt<w_ - shamt> shr() const {
    return bits<w_-1, shamt>();
  }

  template<int other_w>
  SInt<w_> operator>>(const UInt<other_w> &other) const {
    uint64_t dshamt = other.as_single_word();
    SInt<w_> result(ui >> other);
    result.sign_extend(w_ - dshamt);
    return result;
  }

  template<int other_w>
  SInt<w_ + (1<<other_w) - 1> operator<<(const UInt<other_w> &other) const {
    return SInt<w_ + (1<<other_w) - 1>(ui << other);
  }

  UInt<1> operator<=(const SInt<w_> &other) const {
    if (negative()) {
      if (other.negative())
        return ui >= other.ui;
      else
        return UInt<1>(1);
    } else {
      if (other.negative())
        return UInt<1>(0);
      else
        return ui <= other.ui;
    }
  }

  UInt<1> operator>=(const SInt<w_> &other) const {
    if (negative()) {
      if (other.negative())
        return ui <= other.ui;
      else
        return UInt<1>(0);
    } else {
      if (other.negative())
        return UInt<1>(1);
      else
        return ui >= other.ui;
    }
  }

  UInt<1> operator<(const SInt<w_> &other) const {
    return ~(*this >= other);
  }

  UInt<1> operator>(const SInt<w_> &other) const {
    return ~(*this <= other);
  }

  UInt<1> operator==(const SInt<w_> &other) const {
    return ui == other.ui;
  }

  UInt<1> operator!=(const SInt<w_> &other) const {
    return ~(*this == other);
  }


private:
  UInt<w_> ui;

  const static int kWordSize = UInt<w_>::kWordSize;

  bool negative() const {
    return (ui.words_[ui.word_index(w_ - 1)] >> ((w_-1) % kWordSize)) & 1;
  }

  // Direct access for ops that only need small signals
  int64_t as_single_word() const {
    static_assert(w_ <= kWordSize, "SInt too big for single int64_t");
    return ui.words_[0];
  }

  void sign_extend(int sign_index = (w_-1)) {
    int sign_offset = sign_index % kWordSize;
    int sign_word = ui.word_index(sign_index);
    bool is_neg = (ui.words_[sign_word] >> sign_offset) & 1;
    ui.words_[sign_word] = (static_cast<int64_t>(ui.words_[sign_word]) <<
                             (kWordSize - sign_offset - 1)) >>
                             (kWordSize - sign_offset - 1);
    for (int i = sign_word+1; i < ui.NW; i++) {
      ui.words_[i] = is_neg ? -1 : 0;
    }
  }

  void print_to_stream(std::ostream& os) const {
    ui.print_to_stream(os);
  }

  template<int w>
  friend class SInt;

  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const SInt<w>& ui);
};

template<int w>
std::ostream& operator<<(std::ostream& os, const SInt<w>& si) {
  // static_assert(w <= SInt<w>::kWordSize, "SInt too big to print");
  // os << si.as_single_word() << "<S" << w << ">";
  // return os;
  si.print_to_stream(os);
  os << "<S" << w << ">";
  return os;
}

#endif  // SINT_H_
