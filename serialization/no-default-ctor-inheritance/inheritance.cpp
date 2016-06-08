#include "inheritance.h"

const char* Derived1::f() const noexcept
{
    return __PRETTY_FUNCTION__;
}

const char* Derived2::f() const noexcept
{
    return __PRETTY_FUNCTION__;
}
