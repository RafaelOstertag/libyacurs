// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "cursex.h"
#include "vpack.h"

//
// Functors
//

/**
 * Calculate the size hint.
 *
 * Calculate the size hint by finding the max cols.
 */
class VCalcSizeHint {
    private:
	Size __size_hint;

    public:
	VCalcSizeHint(): __size_hint(Size::zero()) {}
	VCalcSizeHint(const VCalcSizeHint& _v): __size_hint(_v.__size_hint) {}
	VCalcSizeHint& operator=(const VCalcSizeHint& _v) {
	    __size_hint=_v.__size_hint;
	    return *this;
	}

	void operator()(const WidgetBase* w) {
	    assert(w!=NULL);
	    __size_hint.cols( w->size_hint().cols() > __size_hint.cols() ? w->size_hint().cols() : __size_hint.cols() );
	}

	const Size& hint() const {
	    return __size_hint;
	}
};

class VSetSizeAvail {
    private:
	const Size& __avail;

    public:
	VSetSizeAvail(const Size& _avail): __avail(_avail) {}
	VSetSizeAvail(const VSetSizeAvail& _s): __avail(_s.__avail) {}
	void operator()(WidgetBase* w) {
	    assert(w!=NULL);
	    w->size_available(__avail);
	}
};

/**
 * Calculates the size required by associated Widgets.
 *
 * It also sets the size_available() of the Widgets and handles dynamically
 * sized Widgets.
 *
 * This class is passed in a std::for_each(). Once for_each() is done, finish()
 * must be called, since that will set size on all dynamically sized Widgets.
 */
class VCalcNSetSize {
    private:
	Size __size;
	Size __size_available;
	std::list<WidgetBase*> __dyn_widgets;

    public:
	VCalcNSetSize(const Size& _av): __size(),
					__size_available(_av),
					__dyn_widgets() {}

	VCalcNSetSize(const VCalcNSetSize& _c): __size(_c.__size),
						__size_available(_c.__size_available),
						__dyn_widgets(_c.__dyn_widgets) {}

	VCalcNSetSize& operator=(const VCalcNSetSize& _c) {
	    __size = _c.__size;
	    __size_available = _c.__size_available;
	    __dyn_widgets = _c.__dyn_widgets;
	    return *this;
	}

	void operator()(WidgetBase* _w) {
	    assert(_w != NULL);

	    // First, reset the size, so that we can identify dynamically sized Widgets
	    _w->resetsize();

	    if ( _w->size() == Size::zero() ) {
		// That's a dynamically sized widget, thus add it to the list
		// for later processing
		__dyn_widgets.push_back(_w);
		return;
	    }

	    __size.rows(__size.rows()+_w->size().rows());
	    __size.cols(__size.cols() < _w->size().cols() ?
			_w->size().cols() : __size.cols());

	    // Also set the size availbe for the widget. Dynamically sized
	    // widgets are handled when CalcNSetSize::size() is called.
	    _w->size_available(_w->size());
	}

	void finish() {
	    if (__dyn_widgets.empty() ) {
		// There are no dynamically sized widgets, there is nothing
		// left to do
		return;
	    }

	    // There are dynamically sized widgets. So let's find out the how
	    // much space is available for them
	    Size remaining_size(__size_available-__size);

	    // We ignore remaining_size.cols() because we vertically stack
	    // widgets and the dynamically sized widgets get the amount of
	    // __size.cols()
	    assert(remaining_size.rows()>0);

	    // This gives the size for each dynamically sized Widget. The
	    // remaining size will be divided equally among all dynamically
	    // sized widgets.
	    Size per_dyn_widget(remaining_size.rows()/__dyn_widgets.size(),
				__size.cols()>0 ? __size.cols() : __size_available.cols());

	    // Set the size for each widget.
	    std::for_each(__dyn_widgets.begin(),
			  __dyn_widgets.end(),
			  VSetSizeAvail(per_dyn_widget));
	}
};

class VSetPosAssoc {
    private:
	Coordinates __pos;
    public:
	VSetPosAssoc(const Coordinates& __start): __pos(__start){}
	VSetPosAssoc(const VSetPosAssoc& _o): __pos(_o.__pos){}
	void operator()(WidgetBase* _w) {
	    _w->position(__pos);
	    // Set the size available
	    _w->size_available(_w->size());

	    __pos.y(__pos.y() + _w->size().rows());
	}
};

//
// Private
//

//
// Protected
//

void
VPack::recalc_size() {
    // It is perfectly legal to have a size_available() of zero when
    // recalc_size() is called. For instance, it happens in such a calling
    // sequence
    //
    //  [...]
    //
    //  Window* window=new Window;
    //
    //  Label* l1=new Label("Label1");
    //  Label* l2=new Label("Label2");
    //
    //  VPack* vp1=new VPack();
    //  vp1->add_back(l1); // The pack has not received any size information
    //  vp1->add_back(l2); // ditto
    //
    //  window->widget(vp1); // Now, the Pack receives size_available().
    //
    //  [...]
    //
    if (WidgetBase::size_available() == Size::zero()) return;

    VCalcNSetSize calc = std::for_each(widget_list.begin(),
				       widget_list.end(),
				       VCalcNSetSize(WidgetBase::size_available()));

    // This is imperative, in order to set the size_available on any
    // dynamically sized Widgets.
    calc.finish();
}

//
// Public
//
VPack::VPack(): Pack() {
}


VPack::VPack(const VPack& _vp): Pack(_vp) {
}

VPack::~VPack() {}

const VPack&
VPack::operator=(const VPack& _vp) {
    Pack::operator=(_vp);
    return *this;
}


Size
VPack::size_hint() const {
    VCalcSizeHint _size_hint;
    _size_hint = std::for_each(widget_list.begin(),
			       widget_list.end(),
			       _size_hint);
    assert(_size_hint.hint().rows()==0);
    return _size_hint.hint();
}

void
VPack::realize() {
    if (realized()) throw AlreadyRealized();

    assert(WidgetBase::size_available()!=Size::zero());

    // Set position for each associated widget. That's the only reason we
    // implement realize() in a derived class.
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  VSetPosAssoc(position()));

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::realize));

    realized(true);
}
