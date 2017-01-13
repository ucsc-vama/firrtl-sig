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
  }
};

int main() {
  return lest::run(spec);
}
