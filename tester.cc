#include <lest.hpp>

#include "uint.h"



UInt<16>  a16(0xcafe);
UInt<16>  b16(0xbebe);
UInt<64>  a64(0xe2bd5b4ff8b30fc8);
UInt<64>  b64(0x2fc353e33c6938a7);
UInt<80>  a80("0x987426c1f7cd7d4d693a");
UInt<80>  b80("0x563a0757a07b7bd27485");
UInt<128> a128("0xe903646a697fcaa344d2b2aa95e47b5d");
UInt<128> b128("0x56fa570ecb04adca42405f12bf28b822");


const lest::test spec[] = {
  CASE("comparison operators") {
    EXPECT( a16 == a16 );
    EXPECT( a16 != b16 );
    EXPECT( b16 <  a16 );
    EXPECT( b16 <= a16 );
    EXPECT( a16 <= a16 );
    EXPECT( a16 >= a16 );
    EXPECT( a16 >  b16 );

    EXPECT( a64 == a64 );
    EXPECT( a64 != b64 );
    EXPECT( b64 <  a64 );
    EXPECT( b64 <= a64 );
    EXPECT( a64 <= a64 );
    EXPECT( a64 >= a64 );
    EXPECT( a64 >  b64 );

    EXPECT( a80 == a80 );
    EXPECT( a80 != b80 );
    EXPECT( b80 <  a80 );
    EXPECT( b80 <= a80 );
    EXPECT( a80 <= a80 );
    EXPECT( a80 >= a80 );
    EXPECT( a80 >  b80 );

    EXPECT( a128 == a128 );
    EXPECT( a128 != b128 );
    EXPECT( b128 <  a128 );
    EXPECT( b128 <= a128 );
    EXPECT( a128 <= a128 );
    EXPECT( a128 >= a128 );
    EXPECT( a128 >  b128 );
  },

  CASE("constructors") {
    EXPECT( a16 == UInt<16>("0xcafe") );
    EXPECT( a16 == UInt<16>(a16) );
    EXPECT( a64 == UInt<64>(a64) );
    EXPECT( a80 == UInt<80>(a80) );
    EXPECT( a128 == UInt<128>(a128) );
  },

  CASE("pad operator") {
    EXPECT( a16 == a16.pad<16>() );
    EXPECT( a16.pad<64>() == UInt<64>(0xcafe) );
    EXPECT( a64.pad<200>() == a64.pad<200>() );
    EXPECT( a64.pad<200>() != b64.pad<200>() );
  },

  CASE("cat operator") {
    EXPECT( a16.cat(b16) == UInt<32>(0xcafebebe) );
    EXPECT( a16.cat(a64) == UInt<80>("0xcafee2bd5b4ff8b30fc8") );
    EXPECT( a64.cat(a16) == UInt<80>("0xe2bd5b4ff8b30fc8cafe") );
    EXPECT( a16.cat(a80) == UInt<96>("0xcafe987426c1f7cd7d4d693a") );
    EXPECT( a80.cat(a16) == UInt<96>("0x987426c1f7cd7d4d693acafe") );
    EXPECT( a16.cat(a128) == UInt<144>("0xcafee903646a697fcaa344d2b2aa95e47b5d") );
    EXPECT( a128.cat(a16) == UInt<144>("0xe903646a697fcaa344d2b2aa95e47b5dcafe") );
    EXPECT( a80.cat(a128) == UInt<208>("0x987426c1f7cd7d4d693ae903646a697fcaa344d2b2aa95e47b5d") );
    EXPECT( a128.cat(a80) == UInt<208>("0xe903646a697fcaa344d2b2aa95e47b5d987426c1f7cd7d4d693a") );
    EXPECT( a128.cat(b128) == UInt<256>("0xe903646a697fcaa344d2b2aa95e47b5d56fa570ecb04adca42405f12bf28b822") );
  },

  CASE("add operator") {
    EXPECT( a16 + b16 == UInt<17>(0x189bc) );
    EXPECT( a64 + b64 == UInt<65>("0x11280af33351c486f") );
    EXPECT( a80 + b80 == UInt<81>("0xeeae2e199848f91fddbf") );
    EXPECT( a128 + b128 == UInt<129>("0x13ffdbb793484786d871311bd550d337f") );
  },

  CASE("sub operator") {
    EXPECT( a16 - b16 == UInt<17>(0xc40) );
    EXPECT( a64 - b64 == UInt<65>("0xb2fa076cbc49d721") );
    EXPECT( a80 - b80 == UInt<81>("0x423a1f6a5752017af4b5") );
    EXPECT( a128 - b128 == UInt<129>("0x92090d5b9e7b1cd902925397d6bbc33b") );
  },

  CASE("mult operator") {
    EXPECT( a16 * b16 == UInt<32>(0x973f2c84) );
    EXPECT( a64 * b64 == UInt<128>("0x2a4dc44ce497c914d9d3df0ec14b0b78") );
    EXPECT( a80 * b80 == UInt<160>("0x335993b54d4bc81d37835773f77fa4765c79f322") );
    EXPECT( a128 * b128 == UInt<256>("0x4f2b00496d758f68469327504061b9045f77243f5cfda64ce9fb69abca8b3a5a") );
  },

  CASE("div operator") {
    EXPECT( a16 / b16 == UInt<16>(1) );
    EXPECT( a64 / b64 == UInt<64>(4) );
  },

  CASE("mod operator") {
    EXPECT( a16 % b16 == UInt<16>(0xc40) );
    EXPECT( a64 % b64 == UInt<64>(0x23b00bc3070e2d2c) );
  },

  CASE("not operator") {
    EXPECT( ~a16 == UInt<16>(0x3501) );
    EXPECT( ~a64 == UInt<64>(0x1d42a4b0074cf037) );
    EXPECT( ~a80 == UInt<80>("0x678bd93e083282b296c5") );
    EXPECT( ~a128 == UInt<128>("0x16fc9b959680355cbb2d4d556a1b84a2") );
  },

  CASE("and operator") {
    EXPECT( (a16 & b16) == UInt<16>(0x8abe) );
    EXPECT( (a64 & b64) == UInt<64>(0x2281534338210880) );
    EXPECT( (a80 & b80) == UInt<80>("0x10300641a04979406000") );
    EXPECT( (a128 & b128) == UInt<128>("0x4002440a490488824040120295203800") );
  },

  CASE("or operator") {
    EXPECT( (a16 | b16) == UInt<16>(0xfefe) );
    EXPECT( (a64 | b64) == UInt<64>(0xefff5beffcfb3fef) );
    EXPECT( (a80 | b80) == UInt<80>("0xde7e27d7f7ff7fdf7dbf") );
    EXPECT( (a128 | b128) == UInt<128>("0xfffb776eeb7fefeb46d2ffbabfecfb7f") );
  },

  CASE("xor operator") {
    EXPECT( (a16 ^ b16) == UInt<16>(0x7440) );
    EXPECT( (a64 ^ b64) == UInt<64>(0xcd7e08acc4da376f) );
    EXPECT( (a80 ^ b80) == UInt<80>("0xce4e219657b6069f1dbf") );
    EXPECT( (a128 ^ b128) == UInt<128>("0xbff93364a27b67690692edb82accc37f") );
  },

  CASE("bits operator") {
    EXPECT( (a16.bits<11,4>()) == UInt<8>(0xaf) );
    EXPECT( (a64.bits<47,24>()) == UInt<24>(0x5b4ff8) );
    EXPECT( (a80.bits<79,64>()) == UInt<16>(0x9874) );
    EXPECT( (a80.bits<71,56>()) == UInt<16>(0x7426) );
    EXPECT( (a128.bits<111,96>()) == UInt<16>(0x646a) );
    EXPECT( (a128.bits<71,56>()) == UInt<16>(0xa344) );
  },

  CASE("head operator") {
    EXPECT( (a16.head<8>()) == UInt<8>(0xca) );
    EXPECT( (a64.head<64>()) == a64 );
    EXPECT( (a64.head<16>()) == UInt<16>(0xe2bd) );
    EXPECT( (a80.head<24>()) == UInt<24>(0x987426) );
    EXPECT( (a128.head<32>()) == UInt<32>(0xe903646a) );
  },

  CASE("tail operator") {
    EXPECT( (a16.tail<8>()) == UInt<8>(0xfe) );
    EXPECT( (a64.tail<0>()) == a64 );
    EXPECT( (a64.tail<16>()) == UInt<48>(0x5b4ff8b30fc8) );
    EXPECT( (a80.tail<8>()) == UInt<72>("0x7426c1f7cd7d4d693a") );
    EXPECT( (a128.tail<32>()) == UInt<96>("0x697fcaa344d2b2aa95e47b5d") );
  },

  CASE("static shifts") {
    EXPECT( a16.shl<0>() == a16 );
    EXPECT( a16.shl<4>() == UInt<20>(0xcafe0) );
    EXPECT( a64.shl<8>() == UInt<72>("0xe2bd5b4ff8b30fc800") );
    EXPECT( a80.shl<60>() == UInt<140>("0x987426c1f7cd7d4d693a000000000000000") );
    EXPECT( a128.shl<72>() == UInt<200>("0xe903646a697fcaa344d2b2aa95e47b5d000000000000000000") );

    EXPECT( a16.shr<0>() == a16 );
    EXPECT( a16.shr<8>() == UInt<8>(0xca) );
    EXPECT( a64.shr<16>() == UInt<48>(0xe2bd5b4ff8b3) );
    EXPECT( a80.shr<24>() == UInt<56>(0x987426c1f7cd7d) );
    EXPECT( a128.shr<48>() == UInt<80>("0xe903646a697fcaa344d2"));
  },

  CASE("dynamic shifts") {
    EXPECT( (a16 << UInt<1>(0)) == UInt<17>(0xcafe) );
    EXPECT( (a16 << UInt<4>(4)) == UInt<31>(0xcafe0) );
    EXPECT( (a64 << UInt<4>(8)) == UInt<79>("0xe2bd5b4ff8b30fc800") );
    EXPECT( (a80 << UInt<5>(12)) == UInt<111>("0x987426c1f7cd7d4d693a000") );
    EXPECT( (a128 << UInt<6>(16)) == UInt<191>("0xe903646a697fcaa344d2b2aa95e47b5d0000") );

    EXPECT( (a16 >> UInt<1>(0)) == UInt<16>(0xcafe) );
    EXPECT( (a16 >> UInt<4>(4)) == UInt<16>(0x0caf) );
    EXPECT( (a64 >> UInt<4>(8)) == UInt<64>("0xe2bd5b4ff8b30f") );
    EXPECT( (a80 >> UInt<5>(12)) == UInt<80>("0x987426c1f7cd7d4d6") );
    EXPECT( (a128 >> UInt<6>(16)) == UInt<128>("0xe903646a697fcaa344d2b2aa95e4") );
  }
};

int main() {
  return lest::run(spec);
}
