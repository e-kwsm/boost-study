#pragma once

#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class C {
 public:
  C() = delete;
  C(int n) : n{n} {}
  ~C() {}

 public:
  friend std::ostream& operator<<(std::ostream& os, const C& c) {
    return os << c.n;
  }
  int get() const noexcept;

 private:
  friend boost::serialization::access;
  template <typename Archive>
  void serialize(Archive& ar, unsigned version);

 private:
  int n;
};

template <typename Archive>
void C::serialize(Archive& ar, unsigned version) {
  // do nothing
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

namespace boost::serialization {
template <typename Archive>
void save_construct_data(Archive& ar, const C* c, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
  int n{c->get()};
  ar << BOOST_SERIALIZATION_NVP(n);
}

template <typename Archive>
void load_construct_data(Archive& ar, C* c, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << std::endl;
  int n;
  ar >> BOOST_SERIALIZATION_NVP(n);
  ::new (c) C{n};
}
}  // namespace boost::serialization
