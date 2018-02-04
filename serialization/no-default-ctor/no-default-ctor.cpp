#include "no-default-ctor.h"

int C::get() const noexcept { return n; }
