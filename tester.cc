#include <lest.hpp>

#include "uint.h"

UInt<16> a(0xcafe);
UInt<16> b(0xbebe);

const lest::test spec[] = {
  CASE("hello")
  {
    EXPECT( a == a );
    EXPECT( a != b );
  }
};

int main() {
  return lest::run(spec);
}
