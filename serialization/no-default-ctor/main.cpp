#include <iostream>
#include <fstream>
#include <memory>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include "no-default-ctor.h"

using namespace std;

int main() {
  constexpr int n = 42;
  constexpr auto fname = "serial.xml";

  {
    auto p = make_unique<C>(n);
    cout << *p << endl;

    std::ofstream ofile{fname};
    boost::archive::xml_oarchive oa{ofile};
    oa << BOOST_SERIALIZATION_NVP(p);
  }
  {
    unique_ptr<C> p;

    std::ifstream ifile{fname};
    boost::archive::xml_iarchive ia{ifile};
    ia >> BOOST_SERIALIZATION_NVP(p);

    assert(n == p->get());
    cout << *p << endl;
  }

  return 0;
}
