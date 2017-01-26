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
  SInt<128> big("0x18cafebebedeadbeef");
  cout << big << endl;
  SInt<16> a(0xcafe);
  SInt<20> b(0xbebe);
  cout << a.cat(b) << endl;
  cout << a.cat(big) << endl;
  cout << SInt<8>(0xb).cat(SInt<128>(0xa)) << endl;
  cout << SInt<8>(0xa0) + SInt<8>(0x0b) << endl;
  cout << SInt<64>(-1) + SInt<64>(1) << endl;
  cout << SInt<60>(-1) * SInt<60>(-1) << endl;
  cout << small * small << endl;
  cout << small / SInt<17>(-2) << " " << small % SInt<17>(4) << endl;
  cout << ~SInt<8>(-1) << " " << ~SInt<8>(0) << endl;
  cout << (SInt<13>(1) & SInt<13>(-1)) << endl;
  cout << (SInt<32>(1) | SInt<32>(-2)) << endl;
  cout << (SInt<64>(-1) ^ SInt<64>(-2)) << endl;
  cout << SInt<10>(1).bits<4,1>() << " " << SInt<80>(-1).bits<72,60>() << endl;
  cout << SInt<10>(1).tail<4>() << " " << SInt<80>(-1).head<20>() << endl;
  cout << (small >> UInt<4>(2)) << " " <<  (SInt<70>(-1) >> UInt<6>(64)) << endl;
  cout << (small << UInt<2>(2)) << " " << (SInt<32>(-1) << UInt<6>(40)) << endl;
  return 0;
}
