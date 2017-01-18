#ifndef SINT_H_
#define SINT_H_

#include <cinttypes>

#include "uint.h"


template<int w_>
class SInt {
public:
  SInt() : ui(0) {}

  SInt(int64_t i) : ui(i) {}

  SInt(std::string initial) : ui(initial) {
    sign_extend();
  }

private:
  UInt<w_> ui;

  const static int kWordSize = UInt<w_>::kWordSize;

  // Direct access for ops that only need small signals
  int64_t as_single_word() const {
    static_assert(w_ <= kWordSize, "SInt too big for single int64_t");
    return ui.words_[0];
  }

  // Clean up high bits
  void sign_extend(int sign_index=ui.bits_in_top_word_) {
    if (ui.bits_in_top_word_ != ui.WW) {
      if (ui.words_[ui.n_-1] & (1l << (ui.bits_in_top_word_ - 1)))
        ui.words_[ui.n_-1] = ui.words_[ui.n_-1] | (-1l << shamt(ui.bits_in_top_word_));
      else
        ui.mask_top_unused();
    }
  }

  void print_to_stream(std::ostream& os) const {
    ui.print_to_stream(os);
  }

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
