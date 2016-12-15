#include <iostream>

#include "uint.h"

using namespace std;

int main() {
  cout << "yo!" << endl;
  cout << sizeof(UInt<1>) << " " << sizeof(UInt<64>) << " " << sizeof(UInt<512>) << endl;
  UInt<32> small(14);
  cout << small << " " << small.widen<40>() << endl;
  UInt<128> big("0x18cafebebedeadbeef");
  cout << big << endl;
  UInt<16> a(0xcafe);
  UInt<20> b(0xbebe);
  cout << a.cat(b) << endl;
  UInt<16> c;
  cout << c << endl;
  c = a;
  cout << c << endl;
  return 0;
}
