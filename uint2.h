#ifndef UINT_H_
#define UINT_H_

#include <array>
#include <cinttypes>


const size_t kWordSize = 64;

template<int w_,
         typename word_t = typename std::conditional<(w_ <= 8), uint8_t, uint64_t>::type,
         int n_ = (w_ <= 8) ? 1 : (w_ + kWordSize - 1) / kWordSize>
class UInt {
private:
  std::array<word_t, n_> values;
};

#endif  // UINT_H_