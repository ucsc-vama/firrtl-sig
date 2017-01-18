#include <iostream>

#include "sint.h"

using namespace std;


int main() {
  cout << sizeof(SInt<1>) << " " << sizeof(SInt<64>) << " " << sizeof(SInt<512>) << endl;
  SInt<17> small(-14);
  cout << small << SInt<65>(-1) << endl;
  cout << SInt<64>(-199703103) << " " << SInt<16>("0xffde") << endl;
  cout << SInt<110>("0x3ffff3f2be72288e0979696bb6e9") << endl;
  cout << SInt<80>(small) << " " << small.pad<80>() << endl;
  UInt<128> big("0x18cafebebedeadbeef");
  cout << big << endl;
  UInt<16> a(0xcafe);
  UInt<20> b(0xbebe);
  cout << a.cat(b) << endl;
  cout << a.cat(big) << endl;
  cout << UInt<8>(0xb).cat(UInt<128>(0xa)) << endl;
  return 0;
}