#include <iostream>

#include "uint2.h"

using namespace std;

int main() {
  cout << "yo!" << endl;
  cout << sizeof(UInt<1>) << " " << sizeof(UInt<64>) << " " << sizeof(UInt<512>) << endl;
  // UInt<32> small(14);
  // cout << small << " " << small.widen<40>() << endl;
  // UInt<128> big("0x18cafebebedeadbeef");
  // cout << big << endl;
  // UInt<16> a(0xcafe);
  // UInt<16> copy_a(a);
  // UInt<80> copy_a_big(a);
  // cout << copy_a << " " << copy_a_big << endl;
  // UInt<20> b(0xbebe);
  // cout << a.cat(b) << endl;
  // cout << a.cat(big) << endl;
  return 0;
}
