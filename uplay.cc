#include <iostream>

#include "uint2.h"

using namespace std;


int main() {
  cout << "yo!" << endl;
  cout << sizeof(UInt<1>) << " " << sizeof(UInt<64>) << " " << sizeof(UInt<512>) << endl;
  UInt<32> small(14);
  cout << small << endl;;
  cout << small.widen<40>() << endl;
  UInt<128> big("0x18cafebebedeadbeef");
  cout << big << endl;
  UInt<16> a(0xcafe);
  UInt<16> copy_a(a);
  UInt<80> copy_a_big(a);
  cout << copy_a << " " << copy_a_big << endl;
  UInt<20> b(0xbebe);
  cout << a.cat(b) << endl;
  cout << a.cat(big) << endl;
  cout << UInt<8>(0xb).cat(UInt<128>(0xa)) << endl;
  cout << UInt<8>(0xa0) + UInt<8>(0x0b) << endl;
  cout << UInt<64>(0xffffffffffffffff) + UInt<1>(1).widen<64>() << endl;
  cout << UInt<7>(0) - UInt<7>(1) << endl;
  cout << UInt<65>("0x10000000000000000") - UInt<65>(1) << endl;
  cout << UInt<65>(0) - UInt<65>(1) << endl;
  return 0;
}
