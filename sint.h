#ifndef SINT_H_
#define SINT_H_

#include <cinttypes>

#include "uint.h"


template<int w_>
class SInt {
public:
  SInt() : ui(0) {}

  SInt(int64_t i) : ui(i) {}

private:
  UInt<w_> ui;

  const static int kWordSize = UInt<w_>::kWordSize;

  // Direct access for ops that only need small signals
  int64_t as_single_word() const {
    static_assert(w_ <= kWordSize, "SInt too big for single int64_t");
    return ui.words_[0];
  }

  template<int w>
  friend std::ostream& operator<<(std::ostream& os, const SInt<w>& ui);
};

template<int w>
std::ostream& operator<<(std::ostream& os, const SInt<w>& si) {
  static_assert(w <= SInt<w>::kWordSize, "SInt too big to print");
  os << si.as_single_word() << "<S" << w << ">";
  return os;
}

#endif  // SINT_H_
