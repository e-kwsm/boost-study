#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include "inheritance.h"

int main() {
  constexpr int n = 42;
  constexpr auto fname = "serial.xml";
  constexpr int d1n = 1;
  constexpr int d2n = 2;

  {
    std::cout << "# serialize\n";
    std::vector<std::shared_ptr<Base>> bs;
    bs.emplace_back(std::make_shared<Derived1>(d1n));
    bs.emplace_back(std::make_shared<Derived2>(d2n));
    for (const auto& b : bs)
      std::cout << *b << ' ' << b->f() << '\n';

    std::ofstream ofile{fname};
    boost::archive::xml_oarchive oa{ofile};
    oa << BOOST_SERIALIZATION_NVP(bs);
  }
  {
    std::cout << "# deserialize\n";
    std::vector<std::shared_ptr<Base>> bs;

    std::ifstream ifile{fname};
    boost::archive::xml_iarchive ia{ifile};
    ia >> BOOST_SERIALIZATION_NVP(bs);

    assert(bs[0]->get() == d1n);
    assert(bs[1]->get() == d2n);
    for (const auto& b : bs)
      std::cout << *b << ' ' << b->f() << '\n';
  }

  return 0;
}
