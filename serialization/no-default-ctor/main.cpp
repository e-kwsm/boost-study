#include <fstream>
#include <iostream>
#include <memory>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include "no-default-ctor.h"

int main() {
  constexpr int n = 42;
  constexpr auto fname = "serial.xml";

  {
    auto p = std::make_unique<C>(n);
    std::cout << *p << '\n';

    std::ofstream ofile{fname};
    boost::archive::xml_oarchive oa{ofile};
    oa << BOOST_SERIALIZATION_NVP(p);
  }
  {
    std::unique_ptr<C> p;

    std::ifstream ifile{fname};
    boost::archive::xml_iarchive ia{ifile};
    ia >> BOOST_SERIALIZATION_NVP(p);

    assert(n == p->get());
    std::cout << *p << '\n';
  }

  return 0;
}
