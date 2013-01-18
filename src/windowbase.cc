// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>
#include <cassert>

#include "curs.h"
#include "cursex.h"
#include "windowbase.h"
#include "eventqueue.h"

//
// Private
//

//
// Protected
//
WINDOW*
WindowBase::getWindow() const {
    return *w;
 }

unsigned int
WindowBase::getInstanceCount() const {
    return *instances;
}

void
WindowBase::setMargin(const Margin& _m) {
    //    assert(!isRealized());
    margin = _m;
}

const Margin&
WindowBase::getMargin() const {
    return margin;
}

void
WindowBase::unrealize() {
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

WindowBase::WindowBase(const Area& _r,
			   const Margin& _m):
    Realizeable(), rect(_r), margin(_m),
    instances(NULL), w(NULL) {

    if ( _r == Area() )
	rect = Curses::inquiryScreenSize();

    w = new WINDOW*;
    *w = NULL;

    instances = new unsigned int;
    *instances = 1;
}

WindowBase::WindowBase(const WindowBase& so):
    Realizeable(so), rect(so.rect), margin(so.margin), instances(so.instances) {
    (*instances)++;
}

WindowBase::~WindowBase() {
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

WindowBase&
WindowBase::operator=(const WindowBase& so) {
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
WindowBase::refresh(bool immediate) {
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
WindowBase::resize(const Area& r) {
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
WindowBase::realize() {
    if (realized()) throw AlreadyRealized();

    assert(rect.x()>=0);
    assert(rect.y()>=0);
    assert(rect.rows()>0);
    assert(rect.cols()>0);

    Area _tmp = rect - margin;

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
