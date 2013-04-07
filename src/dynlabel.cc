// $Id$

#include <cassert>
#include <cstdlib>

#include "dynlabel.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
DynLabel&
DynLabel::operator=(const DynLabel&) {
    throw NotSupported();
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

const std::string&
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
    return Size(1,0);
}

void
DynLabel::reset_size() {
    __size=Size::zero();
}

void
DynLabel::refresh(bool immediate) {
    if (realization()!=REALIZED && realization()!=REALIZING) return;

    assert(widget_subwin()!=0);

    // Make sure there are no left overs in case of text being set by
    // a call to label() case we're realized.

    widget_subwin()->erase();
    YACURS::INTERNAL::CurStr tmp(__label,Coordinates());
    widget_subwin()->addstrx(tmp);

    Widget::refresh(immediate);
}
