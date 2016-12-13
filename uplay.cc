#include <iostream>

#include "uint.h"

using namespace std;

int main() {
  cout << "yo!" << endl;
  cout << sizeof(UInt<1>) << " " << sizeof(UInt<64>) << " " << sizeof(UInt<512>) << endl;
  UInt<32> small(14);
  cout << small << endl;
  return 0;
}
