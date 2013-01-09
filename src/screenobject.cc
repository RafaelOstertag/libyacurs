// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CERRNO
#include <cerrno>
#endif // HAVE_CERRNO

#ifdef HAVE_CASSERT
#include <cassert>
#endif // HAVE_CASSERT

#include "cursex.h"
#include "screenobject.h"
#include "eventqueue.h"

//
// Private
//

//
// Protected
//
WINDOW* 
ScreenObject::getWindow() const {
    return *w;
 }

unsigned int 
ScreenObject::getInstanceCount() const {
    return *instances;
}

//
// Public
//

ScreenObject::ScreenObject(const Margin<> _m): Realizeable(),
					       rect(),
					       margin(_m),
					       instances(NULL),
					       w(NULL) {
    w = new WINDOW*;
    *w = NULL; // resize has to take care of allocating the window

    instances = new unsigned int;
    *instances = 1;
}

ScreenObject::ScreenObject(const ScreenObject& so) : Realizeable(so) {
    instances = so.instances;
    (*instances)++;

    w = so.w;

    rect = so.rect;

    margin = so.margin;

}

ScreenObject::~ScreenObject() {
    if (*instances > 1) {
	(*instances)--;
	return;
    }

    assert(instances!=NULL);

    delete instances;

    assert(w!=NULL);

    if (*w != NULL) {
	int retval = delwin(*w);
	if (retval == ERR)
	    throw DelWindowFailed();
    }

    delete w;
}

ScreenObject&
ScreenObject::operator=(const ScreenObject& so) {
    if ( this == &so)
	return *this;

    Realizeable::operator=(so);

    instances = so.instances;
    assert(instances!=0);
    (*instances)++;

    w = so.w;

    rect = so.rect;

    margin = so.margin;

    return *this;
}

void
ScreenObject::refresh() {
    if (!isRealized()) return;

    assert(w!=NULL);
    assert(*w!=NULL);

    int retval = wnoutrefresh(*w);
    if (retval == ERR)
	throw RefreshFailed();

    return;
}

void
ScreenObject::resize(const Rectangle<>& r) {
    if (!isRealized()) throw NotRealized();

    setRealized(false);

    rect = r;

    int retval = delwin(*w);
    if (retval == ERR) {
	throw DelWindowFailed();
    }

    realize(r);

    refresh();
}

void
ScreenObject::realize(const Rectangle<>& r) {
    if (isRealized()) return;

    rect = r;
    Rectangle<> _tmp = rect - margin;

    *w = newwin(_tmp.getLines(),
		_tmp.getCols(),
		_tmp.getY(),
		_tmp.getX());
    if (*w == NULL) {
	throw NewWindowFailed();
    }

    setRealized(true);
}
