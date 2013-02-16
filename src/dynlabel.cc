// $Id$

#include <cassert>
#include <cstdlib>

#include "dynlabel.h"
#include "cursex.h"

//
// Private
//
DynLabel&
DynLabel::operator=(const DynLabel&) {
    abort();
    return *this;
}

//
// Protected
//
    
//
// Public
//
DynLabel::DynLabel(const std::string& _l): Label(_l) {    
}

DynLabel::~DynLabel() {
}

void
DynLabel::label(const std::string& _l) {
    __label = _l;

    if (realization()==REALIZED) refresh(true);
}

std::string
DynLabel::label() const {
    return __label;
}

void
DynLabel::size_available(const Size& _s) {
    assert(_s.rows()>0);
    WidgetBase::size_available(_s);
    __size=Size(1, _s.cols());
}
    

Size
DynLabel::size_hint() const {
    return Size::zero();
}

void
DynLabel::reset_size() {
    __size=Size::zero();
}

void
DynLabel::refresh(bool immediate) {
    if (realization()!=REALIZED && realization()!=REALIZING) return;

    assert(widget_subwin()!=NULL);

    // Make sure there are no left overs in case of text being set by
    // a call to label() case we're realized.
    if (werase(widget_subwin())==ERR)
	throw EraseFailed();

    if (__label.length()<=static_cast<std::string::size_type>(__size.cols()) ) {
	// if (mymvwaddstr(widget_subwin(), 0, 0,
	// 		    __label.c_str()) == ERR)
	// 	throw AddStrFailed();
	//
	// We ignore the error returned, since the cursor cannot be
	// advanced past the end, and thus the string is
	// truncated. However, the truncation has no effect on label.
	mymvwaddstr(widget_subwin(), 0, 0,  __label.c_str());
    } else {
	if (mymvwaddnstr(widget_subwin(), 0, 0,
			 __label.c_str(), __size.cols()-1)==ERR)
	    throw AddStrFailed();

	if (winsch(widget_subwin(), '>')==ERR)
	    throw WInsChFailed();
    }
	    
		     

    Widget::refresh(immediate);
}
