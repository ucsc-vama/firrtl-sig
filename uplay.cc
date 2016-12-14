#include <iostream>

#include "uint.h"

using namespace std;

int main() {
  cout << "yo!" << endl;
  cout << sizeof(UInt<1>) << " " << sizeof(UInt<64>) << " " << sizeof(UInt<512>) << endl;
  UInt<32> small(14);
  cout << small << " " << small.widen<80>() << endl;
  UInt<128> big("0x18cafebebedeadbeef");
  cout << big << endl;
  return 0;
}
