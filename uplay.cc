#include <iostream>

#include "uint.h"

using namespace std;


int main() {
  cout << "yo!" << endl;
  cout << sizeof(UInt<1>) << " " << sizeof(UInt<64>) << " " << sizeof(UInt<512>) << endl;
  UInt<32> small(14);
  cout << small << endl;;
  cout << small.pad<40>() << endl;
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
  cout << UInt<64>(0xffffffffffffffff) + UInt<1>(1).pad<64>() << endl;
  cout << UInt<64>(0xffffffffffffffff).addw(UInt<64>(1)) << endl;
  cout << UInt<7>(0) - UInt<7>(1) << endl;
  cout << UInt<65>("0x10000000000000000") - UInt<65>(1) << endl;
  cout << UInt<65>(0) - UInt<65>(1) << endl;
  cout << UInt<8>(5) * UInt<8>(5) << endl;
  cout << big * UInt<128>("0x10000000000000001") << endl;
  cout << big * big << endl;
  cout << ~UInt<1>(0) << " " << ~a << " " << ~b << " " << ~big << endl;
  cout << (UInt<2>(2) & UInt<2>(1)) << " " << (a & UInt<16>(0xbebe));
  cout << " " << (big & big) << endl;
  cout << (UInt<2>(2) | UInt<2>(1)) << " " << (a | UInt<16>(0xbebe));
  cout << " " << (big | big) << endl;
  cout << (UInt<2>(2) ^ UInt<2>(1)) << " " << (a ^ UInt<16>(0xbebe));
  cout << " " << (big ^ big) << endl;
  cout << a.bits<11,4>() << " " << big.bits<7,4>() << endl;
  cout << big.cat(big).bits<75,60>() << endl;
  cout << big.cat(big).bits<139,124>() << endl;
  cout << big.cat(big).bits<191,60>() << endl;
  cout << a.head<8>() << " " << a.tail<8>() << endl;
  cout << a.shl<4>() << " " << big.shl<16>() << endl;
  cout << a.shr<4>() << " " << big.shr<16>() << endl;
  cout << (a>>UInt<4>(4)) << " " << (big>>UInt<4>(8)) << endl;
  cout << (a<<UInt<4>(4)) << " " << (big<<UInt<4>(8)) << endl;
  cout << UInt<64>(25) / UInt<8>(4) << " " << UInt<32>(25) % UInt<16>(4) << endl;
  UInt<16> small_b(0xbebe);
  UInt<128> big_plus_1 = (big + UInt<128>(1)).tail<1>();
  cout << (a == a) << " " << (a != a) << " " << (big == big) << endl;
  cout << (a == small_b) << " " << (a != small_b) << endl;
  cout << (a < a) << " " << (a <= a) << " " << (small_b < a) << endl;
  cout << (a > a) << " " << (a >= a) << " " << (a > small_b) << endl;
  cout << (big < big_plus_1) << " " << (big >= big_plus_1) << endl;
  cout << (big > big_plus_1) << " " << (big <= big_plus_1) << endl;
  return 0;
}
