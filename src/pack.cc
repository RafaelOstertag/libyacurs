// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#include "pack.h"

//
// Private
//

//
// Protected
//

//
// Public
//
Pack::Pack(): __size(), widget_list() {}

void
Pack::add(Widget* _w) {
#warning "To be implemented"
    abort();
}

void
Pack::remove(Widget* _w) {
#warning "To be implemented"
    abort();
}
    
void
Pack::size(const Rectangle&) {
    // calling size() on a Pack has no effect
}

const
    
