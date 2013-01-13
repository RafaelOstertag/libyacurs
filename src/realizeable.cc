// $Id$

#include "realizeable.h"

//
// Protected
//

void 
Realizeable::realized(bool _r) {
    __realized = _r;
}

//
// Public
//
Realizeable::Realizeable() : __realized(false) {}

Realizeable::Realizeable(const Realizeable& r) {
    __realized = r.__realized;
}

Realizeable::~Realizeable() {}

Realizeable&
Realizeable::operator=(const Realizeable& r) {
    __realized = r.__realized;
    return *this;
}

bool
Realizeable::realized() const {
    return __realized;
}
