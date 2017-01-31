#include <lest.hpp>

#include "uint.h"
#include "sint.h"


UInt<16>  a16u(0xcafe);
UInt<16>  b16u(0xbebe);
UInt<64>  a64u(0xe2bd5b4ff8b30fc8);
UInt<64>  b64u(0x2fc353e33c6938a7);
UInt<80>  a80u("0x987426c1f7cd7d4d693a");
UInt<80>  b80u("0x563a0757a07b7bd27485");
UInt<128> a128u("0xe903646a697fcaa344d2b2aa95e47b5d");
UInt<128> b128u("0x56fa570ecb04adca42405f12bf28b822");


const lest::test spec[] = {
  CASE("uint comparison operators") {
    EXPECT( a16u == a16u );
    EXPECT( a16u != b16u );
    EXPECT( b16u <  a16u );
    EXPECT( b16u <= a16u );
    EXPECT( a16u <= a16u );
    EXPECT( a16u >= a16u );
    EXPECT( a16u >  b16u );

    EXPECT( a64u == a64u );
    EXPECT( a64u != b64u );
    EXPECT( b64u <  a64u );
    EXPECT( b64u <= a64u );
    EXPECT( a64u <= a64u );
    EXPECT( a64u >= a64u );
    EXPECT( a64u >  b64u );

    EXPECT( a80u == a80u );
    EXPECT( a80u != b80u );
    EXPECT( b80u <  a80u );
    EXPECT( b80u <= a80u );
    EXPECT( a80u <= a80u );
    EXPECT( a80u >= a80u );
    EXPECT( a80u >  b80u );

    EXPECT( a128u == a128u );
    EXPECT( a128u != b128u );
    EXPECT( b128u <  a128u );
    EXPECT( b128u <= a128u );
    EXPECT( a128u <= a128u );
    EXPECT( a128u >= a128u );
    EXPECT( a128u >  b128u );
  },

  CASE("uint constructors") {
    EXPECT( a16u == UInt<16>("0xcafe") );
    EXPECT( a16u == UInt<16>(a16u) );
    EXPECT( a64u == UInt<64>(a64u) );
    EXPECT( a80u == UInt<80>(a80u) );
    EXPECT( a128u == UInt<128>(a128u) );
  },

  CASE("uint pad operator") {
    EXPECT( a16u == a16u.pad<16>() );
    EXPECT( a16u.pad<64>() == UInt<64>(0xcafe) );
    EXPECT( a64u.pad<200>() == a64u.pad<200>() );
    EXPECT( a64u.pad<200>() != b64u.pad<200>() );
  },

  CASE("uint cat operator") {
    EXPECT( a16u.cat(b16u) == UInt<32>(0xcafebebe) );
    EXPECT( a16u.cat(a64u) == UInt<80>("0xcafee2bd5b4ff8b30fc8") );
    EXPECT( a64u.cat(a16u) == UInt<80>("0xe2bd5b4ff8b30fc8cafe") );
    EXPECT( a16u.cat(a80u) == UInt<96>("0xcafe987426c1f7cd7d4d693a") );
    EXPECT( a80u.cat(a16u) == UInt<96>("0x987426c1f7cd7d4d693acafe") );
    EXPECT( a16u.cat(a128u) == UInt<144>("0xcafee903646a697fcaa344d2b2aa95e47b5d") );
    EXPECT( a128u.cat(a16u) == UInt<144>("0xe903646a697fcaa344d2b2aa95e47b5dcafe") );
    EXPECT( a80u.cat(a128u) == UInt<208>("0x987426c1f7cd7d4d693ae903646a697fcaa344d2b2aa95e47b5d") );
    EXPECT( a128u.cat(a80u) == UInt<208>("0xe903646a697fcaa344d2b2aa95e47b5d987426c1f7cd7d4d693a") );
    EXPECT( a128u.cat(b128u) == UInt<256>("0xe903646a697fcaa344d2b2aa95e47b5d56fa570ecb04adca42405f12bf28b822") );
  },

  CASE("uint add operator") {
    EXPECT( a16u + b16u == UInt<17>(0x189bc) );
    EXPECT( a64u + b64u == UInt<65>("0x11280af33351c486f") );
    EXPECT( a80u + b80u == UInt<81>("0xeeae2e199848f91fddbf") );
    EXPECT( a128u + b128u == UInt<129>("0x13ffdbb793484786d871311bd550d337f") );
  },

  CASE("uint addw operator") {
    EXPECT( a16u.addw(b16u) == UInt<16>(0x89bc) );
    EXPECT( a64u.addw(b64u) == UInt<64>("0x1280af33351c486f") );
    EXPECT( a80u.addw(b80u) == UInt<80>("0xeeae2e199848f91fddbf") );
    EXPECT( a128u.addw(b128u) == UInt<128>("0x3ffdbb793484786d871311bd550d337f") );
  },

  CASE("uint sub operator") {
    EXPECT( a16u - b16u == SInt<17>(0xc40) );
    EXPECT( a64u - b64u == SInt<65>("0xb2fa076cbc49d721") );
    EXPECT( a80u - b80u == SInt<81>("0x423a1f6a5752017af4b5") );
    EXPECT( a128u - b128u == SInt<129>("0x92090d5b9e7b1cd902925397d6bbc33b") );
  },

  CASE("uint mult operator") {
    EXPECT( a16u * b16u == UInt<32>(0x973f2c84) );
    EXPECT( a64u * b64u == UInt<128>("0x2a4dc44ce497c914d9d3df0ec14b0b78") );
    EXPECT( a80u * b80u == UInt<160>("0x335993b54d4bc81d37835773f77fa4765c79f322") );
    EXPECT( a128u * b128u == UInt<256>("0x4f2b00496d758f68469327504061b9045f77243f5cfda64ce9fb69abca8b3a5a") );
  },

  CASE("uint div operator") {
    EXPECT( a16u / b16u == UInt<16>(1) );
    EXPECT( a64u / b64u == UInt<64>(4) );
  },

  CASE("uint mod operator") {
    EXPECT( a16u % b16u == UInt<16>(0xc40) );
    EXPECT( a64u % b64u == UInt<64>(0x23b00bc3070e2d2c) );
  },

  CASE("uint not operator") {
    EXPECT( ~a16u == UInt<16>(0x3501) );
    EXPECT( ~a64u == UInt<64>(0x1d42a4b0074cf037) );
    EXPECT( ~a80u == UInt<80>("0x678bd93e083282b296c5") );
    EXPECT( ~a128u == UInt<128>("0x16fc9b959680355cbb2d4d556a1b84a2") );
  },

  CASE("uint and operator") {
    EXPECT( (a16u & b16u) == UInt<16>(0x8abe) );
    EXPECT( (a64u & b64u) == UInt<64>(0x2281534338210880) );
    EXPECT( (a80u & b80u) == UInt<80>("0x10300641a04979406000") );
    EXPECT( (a128u & b128u) == UInt<128>("0x4002440a490488824040120295203800") );
  },

  CASE("uint or operator") {
    EXPECT( (a16u | b16u) == UInt<16>(0xfefe) );
    EXPECT( (a64u | b64u) == UInt<64>(0xefff5beffcfb3fef) );
    EXPECT( (a80u | b80u) == UInt<80>("0xde7e27d7f7ff7fdf7dbf") );
    EXPECT( (a128u | b128u) == UInt<128>("0xfffb776eeb7fefeb46d2ffbabfecfb7f") );
  },

  CASE("uint xor operator") {
    EXPECT( (a16u ^ b16u) == UInt<16>(0x7440) );
    EXPECT( (a64u ^ b64u) == UInt<64>(0xcd7e08acc4da376f) );
    EXPECT( (a80u ^ b80u) == UInt<80>("0xce4e219657b6069f1dbf") );
    EXPECT( (a128u ^ b128u) == UInt<128>("0xbff93364a27b67690692edb82accc37f") );
  },

  CASE("uint bits operator") {
    EXPECT( (a16u.bits<11,4>()) == UInt<8>(0xaf) );
    EXPECT( (a64u.bits<47,24>()) == UInt<24>(0x5b4ff8) );
    EXPECT( (a80u.bits<79,64>()) == UInt<16>(0x9874) );
    EXPECT( (a80u.bits<71,56>()) == UInt<16>(0x7426) );
    EXPECT( (a128u.bits<111,96>()) == UInt<16>(0x646a) );
    EXPECT( (a128u.bits<71,56>()) == UInt<16>(0xa344) );
  },

  CASE("uint head operator") {
    EXPECT( (a16u.head<8>()) == UInt<8>(0xca) );
    EXPECT( (a64u.head<64>()) == a64u );
    EXPECT( (a64u.head<16>()) == UInt<16>(0xe2bd) );
    EXPECT( (a80u.head<24>()) == UInt<24>(0x987426) );
    EXPECT( (a128u.head<32>()) == UInt<32>(0xe903646a) );
  },

  CASE("uint tail operator") {
    EXPECT( (a16u.tail<8>()) == UInt<8>(0xfe) );
    EXPECT( (a64u.tail<0>()) == a64u );
    EXPECT( (a64u.tail<16>()) == UInt<48>(0x5b4ff8b30fc8) );
    EXPECT( (a80u.tail<8>()) == UInt<72>("0x7426c1f7cd7d4d693a") );
    EXPECT( (a128u.tail<32>()) == UInt<96>("0x697fcaa344d2b2aa95e47b5d") );
  },

  CASE("uint static shifts") {
    EXPECT( a16u.shl<0>() == a16u );
    EXPECT( a16u.shl<4>() == UInt<20>(0xcafe0) );
    EXPECT( a64u.shl<8>() == UInt<72>("0xe2bd5b4ff8b30fc800") );
    EXPECT( a80u.shl<60>() == UInt<140>("0x987426c1f7cd7d4d693a000000000000000") );
    EXPECT( a128u.shl<72>() == UInt<200>("0xe903646a697fcaa344d2b2aa95e47b5d000000000000000000") );

    EXPECT( a16u.shr<0>() == a16u );
    EXPECT( a16u.shr<8>() == UInt<8>(0xca) );
    EXPECT( a64u.shr<16>() == UInt<48>(0xe2bd5b4ff8b3) );
    EXPECT( a80u.shr<24>() == UInt<56>(0x987426c1f7cd7d) );
    EXPECT( a128u.shr<48>() == UInt<80>("0xe903646a697fcaa344d2"));
  },

  CASE("uint dynamic shifts") {
    EXPECT( (a16u << UInt<1>(0)) == UInt<17>(0xcafe) );
    EXPECT( (a16u << UInt<4>(4)) == UInt<31>(0xcafe0) );
    EXPECT( (a64u << UInt<4>(8)) == UInt<79>("0xe2bd5b4ff8b30fc800") );
    EXPECT( (a80u << UInt<5>(12)) == UInt<111>("0x987426c1f7cd7d4d693a000") );
    EXPECT( (a128u << UInt<6>(16)) == UInt<191>("0xe903646a697fcaa344d2b2aa95e47b5d0000") );

    EXPECT( (a16u >> UInt<1>(0)) == UInt<16>(0xcafe) );
    EXPECT( (a16u >> UInt<4>(4)) == UInt<16>(0x0caf) );
    EXPECT( (a64u >> UInt<4>(8)) == UInt<64>("0xe2bd5b4ff8b30f") );
    EXPECT( (a80u >> UInt<5>(12)) == UInt<80>("0x987426c1f7cd7d4d6") );
    EXPECT( (a128u >> UInt<6>(16)) == UInt<128>("0xe903646a697fcaa344d2b2aa95e4") );
  },

  CASE("uint conversion") {
    EXPECT( a16u.asUInt() == a16u );
    EXPECT( a16u.asSInt() == SInt<16>("0xcafe") );
    EXPECT( a16u.cvt() == SInt<17>(0xcafe) );
  }
};

int main() {
  return lest::run(spec);
}
