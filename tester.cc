#include <lest.hpp>

const lest::test spec[] = {
  CASE("hello")
  {
    EXPECT( 1 < 2 );
  }
};

int main() {
  return lest::run(spec);
}
