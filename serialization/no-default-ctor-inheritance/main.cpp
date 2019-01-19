#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include "inheritance.h"

using namespace std;

int main() {
  constexpr int n = 42;
  constexpr auto fname = "serial.xml";
  constexpr int d1n = 1;
  constexpr int d2n = 2;

  {
    cout << "# serialize\n";
    vector<shared_ptr<Base>> bs;
    bs.emplace_back(make_shared<Derived1>(d1n));
    bs.emplace_back(make_shared<Derived2>(d2n));
    for (const auto& b : bs) cout << *b << ' ' << b->f() << endl;

    std::ofstream ofile{fname};
    boost::archive::xml_oarchive oa{ofile};
    oa << BOOST_SERIALIZATION_NVP(bs);
  }
  {
    cout << "# deserialize\n";
    vector<shared_ptr<Base>> bs;

    std::ifstream ifile{fname};
    boost::archive::xml_iarchive ia{ifile};
    ia >> BOOST_SERIALIZATION_NVP(bs);

    assert(bs[0]->get() == d1n);
    assert(bs[1]->get() == d2n);
    for (const auto& b : bs) cout << *b << ' ' << b->f() << endl;
  }

  return 0;
}
