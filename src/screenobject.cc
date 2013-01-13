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

#include "curs.h"
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

void
ScreenObject::setMargin(const Margin<>& _m) {
    //    assert(!isRealized());
    margin = _m;
}

const Margin<>&
ScreenObject::getMargin() const {
    return margin;
}

void
ScreenObject::unrealize() {
    if (not realized()) throw NotRealized();

    realized(false);

    assert(w!=NULL);
    assert(*w!=NULL);

    if (delwin(*w) == ERR)
	throw DelWindowFailed();

    *w = NULL;
}


//
// Public
//

ScreenObject::ScreenObject(const Rectangle<>& _r,
			   const Margin<>& _m):
    Realizeable(), rect(_r), margin(_m),
    instances(NULL), w(NULL) {

    if ( _r == Rectangle<>() )
	rect = Curses::inquiryScreenSize();

    w = new WINDOW*;
    *w = NULL;

    instances = new unsigned int;
    *instances = 1;
}

ScreenObject::ScreenObject(const ScreenObject& so):
    Realizeable(so), rect(so.rect), margin(so.margin), instances(so.instances) {
    (*instances)++;
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
    assert(instances!=NULL);
    (*instances)++;

    w = so.w;

    rect = so.rect;

    margin = so.margin;

    return *this;
}

void
ScreenObject::refresh(bool immediate) {
    if (!realized()) return;

    assert(w!=NULL);
    assert(*w!=NULL);

    int retval;
    if (immediate)
	retval = wrefresh(*w);
    else
	retval = wnoutrefresh(*w);

    if (retval == ERR)
	throw RefreshFailed();

}

void
ScreenObject::resize(const Rectangle<>& r) {
    //
    // Keep in mind: a resize does not refresh!
    //
    if (!realized()) throw NotRealized();

    assert(r.x()>=0);
    assert(r.y()>=0);
    assert(r.rows()>0);
    assert(r.cols()>0);

    unrealize();

    rect = r;

    realize();
}

void
ScreenObject::realize() {
    if (realized()) throw AlreadyRealized();

    assert(rect.x()>=0);
    assert(rect.y()>=0);
    assert(rect.rows()>0);
    assert(rect.cols()>0);

    Rectangle<> _tmp = rect - margin;

    assert(_tmp.x()>=0);
    assert(_tmp.y()>=0);
    assert(_tmp.rows()>0);
    assert(_tmp.cols()>0);


    assert(w!=NULL);
    assert(*w==NULL);
    *w = newwin(_tmp.rows(),
		_tmp.cols(),
		_tmp.y(),
		_tmp.x());
    if (*w == NULL) {
	throw NewWindowFailed();
    }

    realized(true);
}
