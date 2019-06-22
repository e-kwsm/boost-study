#pragma once

#include <iostream>
#include <boost/serialization/access.hpp>

class Base {
 public:
  Base() = delete;
  Base(int n) : n{n} {}
  virtual ~Base() {}

 public:
  friend std::ostream& operator<<(std::ostream& os, const Base& b) { return os << b.n; }

 public:
  virtual const char* f() const = 0;
  int get() const noexcept { return n; }

 public:
  template<class Archive>
  void SaveConstDataCommon(Archive& ar, unsigned version) const;

 protected:
  friend boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned version);

 protected:
  int n;
};

class Derived1 : public Base {
 public:
  Derived1() = delete;
  Derived1(int n) : Base{n} {}
  ~Derived1() {}

 public:
  const char* f() const noexcept override final;

 private:
  friend boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned version);
};

class Derived2 : public Base {
 public:
  Derived2() = delete;
  Derived2(int n) : Base{n} {}
  ~Derived2() {}

 public:
  const char* f() const noexcept override final;

 private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned version);
};

#include "serial.h"
