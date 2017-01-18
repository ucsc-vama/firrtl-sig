#include <iostream>

#include "sint.h"

using namespace std;


int main() {
  // cout << "yo!" << endl;
  cout << sizeof(SInt<1>) << " " << sizeof(SInt<64>) << " " << sizeof(SInt<512>) << endl;
  SInt<32> small(-14);
  cout << small << endl;
  // cout << small.pad<40>() << endl;
}
