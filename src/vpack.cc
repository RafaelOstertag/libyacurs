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
 * Functor for calculating the size hint by finding the max columns
 * size and summing up row size.
 *
 * Packs may return rows()>0 or cols()>0 as hint, as opposed to other
 * widget that only return one of the components >0.
 */
class VGetMaxSizeHint {
    private:
	Size __size_max;

    public:
	VGetMaxSizeHint(): __size_max(Size::zero()) {}
	VGetMaxSizeHint(const VGetMaxSizeHint& _v):
	    __size_max(_v.__size_max) {}
	VGetMaxSizeHint& operator=(const VGetMaxSizeHint& _v) {
	    __size_max=_v.__size_max;
	    return *this;
	}

	void operator()(const WidgetBase* w) {
	    assert(w!=NULL);
	    __size_max.cols(std::max(w->size_hint().cols(),
				     __size_max.cols()));
	    __size_max.rows(w->size_hint().rows()+__size_max.rows());
	}

	const Size& hint() const {
	    return __size_max;
	}
};

/**
 * Set size_available() on widgets.
 *
 * Functor for setting available size on Widgets.
 */
class VSetSizeAvail {
    private:
	const Size& __avail;

    public:
	VSetSizeAvail(const Size& _avail): __avail(_avail) {}
	VSetSizeAvail(const VSetSizeAvail& _s): __avail(_s.__avail) {}
	void operator()(WidgetBase* w) {
	    assert(w!=NULL);
	    assert(__avail.rows()>0);
	    assert(__avail.cols()>0);
	    w->size_available(__avail);
	}
};

/**
 * Set size_available on hinted Widgets.
 *
 * Functor for setting size_available on hinted Widgets.
 */
class VSetSizeHinted {
    private:
	Size __const_cols;
	Size __size_used;

    public:
	VSetSizeHinted(const Size& _cc): __const_cols(_cc),
					 __size_used() {}
	VSetSizeHinted(const VSetSizeHinted& _h):
	    __const_cols(_h.__const_cols),
	    __size_used(_h.__size_used) {}
	VSetSizeHinted& operator=(const VSetSizeHinted& _h) {
	    __const_cols=_h.__const_cols;
	    __size_used=_h.__size_used;
	    return *this;
	}

	void operator()(WidgetBase* _w) {
	    assert(_w!=NULL);
	    assert(_w->size_hint().rows()>0);
	    assert(__const_cols.cols()>0);

	    // _sa is supposed to hold the constant row value and the
	    // hinted cols value as retrieved from calling size_hint()
	    // on the Widget
	    Size _sa(__const_cols);
	    _sa.rows(_w->size_hint().rows());

	    // Finally, set the available size on the Widget
	    _w->size_available(_sa);

	    // Add the hinted cols to __size_used, so that they can be
	    // deducted from the total size available to the pack.
	    __size_used.rows(__size_used.rows()+_w->size_hint().rows());
	}

	const Size& size_used() const {
	    return __size_used;
	}
};

/**
 * Calculates and sets the available size for associated widgets.
 *
 * This class is passed in a std::for_each(). Once for_each() is done,
 * finish() must be called, since that will set size on all
 * dynamically sized Widgets.
 */
class VCalcNSetSize {
    private:
	Size __size;
	Size __size_available;
	std::list<WidgetBase*> __dyn_widgets;
	std::list<WidgetBase*> __hinted_widgets;

    public:
	VCalcNSetSize(const Size& _av): __size(),
					__size_available(_av),
					__dyn_widgets(),
					__hinted_widgets() {}

	VCalcNSetSize(const VCalcNSetSize& _c):
	    __size(_c.__size),
	    __size_available(_c.__size_available),
	    __dyn_widgets(_c.__dyn_widgets),
	    __hinted_widgets(_c.__hinted_widgets) {}

	VCalcNSetSize& operator=(const VCalcNSetSize& _c) {
	    __size = _c.__size;
	    __size_available = _c.__size_available;
	    __dyn_widgets = _c.__dyn_widgets;
	    __hinted_widgets = _c.__hinted_widgets;
	    return *this;
	}

	void operator()(WidgetBase* _w) {
	    assert(_w != NULL);

	    // First, reset the size, so that we can identify
	    // dynamically sized Widgets
	    _w->reset_size();

	    if ( _w->size() == Size::zero() ) {
		// That's a dynamically sized widget, thus add it to
		// the list for later processing

		// if it is capable of giving a hint, add it to the
		// __hinted_widgets list which is treated separately
		// from __dyn_widgets.
		if (_w->size_hint().rows()>0)
		    __hinted_widgets.push_back(_w);
		else
		    __dyn_widgets.push_back(_w);

		return;
	    }

	    __size.rows(__size.rows()+_w->size().rows());
	    __size.cols(std::max(__size.cols(),_w->size().cols()) );

	    // Also set the size availabe for the widget. Dynamically
	    // sized widgets are handled when CalcNSetSize::finish()
	    // is called.
	    _w->size_available(_w->size());
	}

	void finish() {
	    if (__dyn_widgets.empty() && __hinted_widgets.empty() ) {
		// There are no dynamically sized widgets, there is
		// nothing left to do
		return;
	    }


	    // In order to process hinted widgets, get the cols. We
	    // only consider Widgets hinting on rows
	    Size cols_unhinted(0, __size.cols()>0 ?
			       __size.cols() :
			       __size_available.cols());
	    assert(cols_unhinted.cols()>0);

	    VSetSizeHinted hinted_size(std::for_each(__hinted_widgets.begin(),
						     __hinted_widgets.end(),
						     VSetSizeHinted(cols_unhinted)) );

	    // Bail out if there are no unhinted dynamic widgets
	    if (__dyn_widgets.empty()) return;

	    // There are dynamically sized widgets. So let's find out
	    // the how much space is available for them
	    Size remaining_size(__size_available-__size);

	    remaining_size-=hinted_size.size_used();

	    // We ignore remaining_size.cols() because we vertically
	    // stack widgets and the dynamically sized widgets get the
	    // amount of __size.cols()
	    assert(remaining_size.rows()>0);

	    // This gives the size for each dynamically sized
	    // Widget. The remaining size will be divided equally
	    // among all dynamically sized widgets.
	    Size per_dyn_widget(remaining_size.rows()/__dyn_widgets.size(),
				__size.cols()>0 ?
				__size.cols() :
				__size_available.cols());

	    // Set the size for each widget.
	    std::for_each(__dyn_widgets.begin(),
			  __dyn_widgets.end(),
			  VSetSizeAvail(per_dyn_widget));
	}
};

/**
 * Functor calculating the size only if no dynamically sized Widgets
 * are associated. Else it will return Size::zero().
 *
 * Used in calc_size().
 */
class VCalcSize {
    private:
	Size __size;
	bool __had_dynamic;
    public:
	VCalcSize(): __size(), __had_dynamic(false) {}
	VCalcSize(const VCalcSize& _r): __size(_r.__size),
					__had_dynamic(_r.__had_dynamic){}
	VCalcSize& operator=(const VCalcSize& _r) {
	    __size = _r.__size;
	    __had_dynamic = _r.__had_dynamic;
	    return *this;
	}
	void operator()(const WidgetBase* _w) {
	    assert(_w!=NULL);

	    // Do nothing if we already found a dynamic widget
	    if (__had_dynamic) return;

	    if (_w->size() == Size::zero() ) {
		// found a dynamic widget. Reset size and mark
		__had_dynamic = true;
		__size = Size::zero();
		return;
	    }

	    __size.rows(__size.rows()+_w->size().rows());
	    __size.cols(std::max(__size.cols(),_w->size().cols()));
	}
	const Size& size() const {
	    return __size;
	}
};

class VSetPosWidget {
    private:
	Coordinates __pos;
    public:
	VSetPosWidget(const Coordinates& __start): __pos(__start){}
	VSetPosWidget(const VSetPosWidget& _o): __pos(_o.__pos){}
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
    // recalc_size() is called. For instance, it happens in such a
    // calling sequence
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

    VCalcNSetSize calc(WidgetBase::size_available());
    calc = std::for_each(widget_list.begin(),
			 widget_list.end(),
			 calc);

    // This is imperative, in order to set the size_available on any
    // dynamically sized Widgets.
    calc.finish();
}

Size
VPack::calc_size() const {
    VCalcSize _s;
    _s=std::for_each(widget_list.begin(),
		     widget_list.end(),
		     _s);
    return _s.size();
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
    if (!hinting()) return Size::zero();

    // remember that Packs may return either component >0 when
    // hinting, see also comment on VGetMaxSizeHint
    VGetMaxSizeHint shint;

    shint = std::for_each(widget_list.begin(),
			  widget_list.end(),
			  shint); 
    return shint.hint();
}

void
VPack::realize() {
    if (realized()) throw AlreadyRealized();

    assert(WidgetBase::size_available().rows()>0);
    assert(WidgetBase::size_available().cols()>0);

    recalc_size();

    // Set position for each associated widget. That's the only reason
    // we implement realize() in a derived class.
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  VSetPosWidget(position()));

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::realize));

    realized(true);
}
