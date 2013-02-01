// $Id$

#include <cassert>

#include "realizeable.h"

//
// Protected
//

void 
Realizeable::realization(REALIZATION_STATE _r) {
    __realization_state = _r;
}

//
// Public
//
Realizeable::Realizeable() : __realization_state(UNREALIZED) {}

Realizeable::Realizeable(const Realizeable& r) {
    __realization_state = r.__realization_state;
}

Realizeable::~Realizeable() {}

Realizeable&
Realizeable::operator=(const Realizeable& r) {
    __realization_state = r.__realization_state;
    return *this;
}

REALIZATION_STATE
Realizeable::realization() const {
    return __realization_state;
}
