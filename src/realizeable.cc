// $Id$

#include <cassert>

#include "realizeable.h"

using namespace YACURS;

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

Realizeable::~Realizeable() {}

REALIZATION_STATE
Realizeable::realization() const {
    return __realization_state;
}
