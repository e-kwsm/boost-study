#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/nvp.hpp>

// after archive!
BOOST_CLASS_EXPORT_KEY(Derived1);
BOOST_CLASS_EXPORT_KEY(Derived2);

template<class Archive>
void Base::SaveConstDataCommon(Archive& ar, unsigned version) const {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  ar << BOOST_SERIALIZATION_NVP(n);
}

template<class Archive>
void Base::serialize(Archive& ar, unsigned version) {
  // do nothing
  std::cerr << __PRETTY_FUNCTION__ << '\n';
}

template<class Archive>
void Derived1::serialize(Archive& ar, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base);
}

template<class Archive>
void Derived2::serialize(Archive& ar, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  ar& BOOST_SERIALIZATION_BASE_OBJECT_NVP(Base);
}

namespace boost::serialization {
template<class Archive>
void save_construct_data(Archive& ar, const Derived1* d1, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  d1->SaveConstDataCommon(ar, version);
}

template<class Archive>
void save_construct_data(Archive& ar, const Derived2* d2, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  d2->SaveConstDataCommon(ar, version);
}

template<class Archive>
void load_construct_data(Archive& ar, Derived1* d1, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  int n;
  ar >> BOOST_SERIALIZATION_NVP(n);
  ::new (d1) Derived1{n};
}

template<class Archive>
void load_construct_data(Archive& ar, Derived2* d2, unsigned version) {
  std::cerr << __PRETTY_FUNCTION__ << '\n';
  int n;
  ar >> BOOST_SERIALIZATION_NVP(n);
  ::new (d2) Derived2{n};
}
}  // namespace boost::serialization
