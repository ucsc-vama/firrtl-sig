#include <iostream>

#include "sint.h"

using namespace std;


int main() {
  cout << sizeof(SInt<1>) << " " << sizeof(SInt<64>) << " " << sizeof(SInt<512>) << endl;
  SInt<17> small(-14);
  cout << small << endl;
  cout << SInt<65>(-1) << endl;
}
