// $Id$

#include "realizeable.h"

//
// Protected
//

void 
Realizeable::setRealized(bool _r) {
    realized = _r;
}

//
// Public
//
Realizeable::Realizeable() : realized(false) {}

Realizeable::Realizeable(const Realizeable& r) {
    realized = r.realized;
}

Realizeable::~Realizeable() {}

Realizeable&
Realizeable::operator=(const Realizeable& r) {
    realized = r.realized;
    return *this;
}

bool
Realizeable::isRealized() const {
    return realized;
}
