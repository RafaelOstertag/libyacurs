// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "cursex.h"
#include "hpack.h"

//
// Functors
//

/**
 * Calculate the size hint.
 *
 * Functor for calculating the size hint by finding the max rows.
 */
class HCalcSizeHint {
    private:
	Size __size_hint;

    public:
	HCalcSizeHint(): __size_hint(Size::zero()) {}
	HCalcSizeHint(const HCalcSizeHint& _v): __size_hint(_v.__size_hint) {}
	HCalcSizeHint& operator=(const HCalcSizeHint& _v) {
	    __size_hint=_v.__size_hint;
	    return *this;
	}

	void operator()(const WidgetBase* w) {
	    assert(w!=NULL);
	    __size_hint.rows( w->size_hint().cols() > __size_hint.cols() ? w->size_hint().cols() : __size_hint.cols() );
	}

	const Size& hint() const {
	    return __size_hint;
	}
};

/**
 * Set size_available() on widgets.
 *
 * Functor for setting available size on Widgets.
 */
class HSetSizeAvail {
    private:
	const Size& __avail;

    public:
	HSetSizeAvail(const Size& _avail): __avail(_avail) {}
	HSetSizeAvail(const HSetSizeAvail& _s): __avail(_s.__avail) {}
	void operator()(WidgetBase* w) {
	    assert(w!=NULL);
	    w->size_available(__avail);
	}
};

/**
 * Set size_available on hinted Widgets.
 *
 * Functor for setting size_available on hinted Widgets.
 */
class HSetSizeHinted {
    private:
	Size __const_rows;
	Size __size_used;

    public:
	HSetSizeHinted(const Size& _cr): __const_rows(_cr),
					 __size_used() {}
	HSetSizeHinted(const HSetSizeHinted& _h): __const_rows(_h.__const_rows),
						  __size_used(_h.__size_used) {}
	HSetSizeHinted& operator=(const HSetSizeHinted& _h) {
	    __const_rows=_h.__const_rows;
	    __size_used=_h.__size_used;
	    return *this;
	}

	void operator()(WidgetBase* _w) {
	    assert(_w!=NULL);
	    assert(_w->size_hint().cols()>0);

	    // _sa is supposed to hold the constant row value and the
	    // hinted cols value as retrieved from calling size_hint()
	    // on the Widget
	    Size _sa(__const_rows);
	    _sa.cols(_w->size_hint().cols());

	    // Finally, set the available size on the Widget
	    _w->size_available(_sa);

	    // Add the hinted cols to __size_used, so that they can be
	    // deducted from the total size available to the pack.
	    __size_used.cols(__size_used.cols()+_w->size_hint().cols());
	}

	const Size& size_used() const {
	    return __size_used;
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
class HCalcNSetSize {
    private:
	Size __size;
	Size __size_available;
	std::list<WidgetBase*> __dyn_widgets;
	std::list<WidgetBase*> __hinted_widgets;

    public:
	HCalcNSetSize(const Size& _av): __size(),
				       __size_available(_av),
					__dyn_widgets(),
					__hinted_widgets() {}

	HCalcNSetSize(const HCalcNSetSize& _c): __size(_c.__size),
						__size_available(_c.__size_available),
						__dyn_widgets(_c.__dyn_widgets),
						__hinted_widgets() {}

	HCalcNSetSize& operator=(const HCalcNSetSize& _c) {
	    __size = _c.__size;
	    __size_available = _c.__size_available;
	    __dyn_widgets = _c.__dyn_widgets;
	    __hinted_widgets = _c.__hinted_widgets;
	    return *this;
	}


	void operator()(WidgetBase* _w) {
	    assert(_w != NULL);

	    // First, reset the size, so that we can identify dynamically sized Widgets
	    _w->resetsize();

	    if ( _w->size() == Size::zero() ) {
		// That's a dynamically sized widget, thus add it to the list
		// for later processing

		// if it is capable of giving a hint, add it to the
		// __hinted_widgets list which is treated separately
		// from __dyn_widgets.
		if (_w->size_hint().cols()>0)
		    __hinted_widgets.push_back(_w);
		else
		    __dyn_widgets.push_back(_w);

		return;
	    }


	    __size.rows(__size.rows() < _w->size().rows() ?
			_w->size().rows() : __size.rows());
	    __size.cols(__size.cols()+_w->size().cols());

	    // Also set the size availabe for the widget. Dynamically sized
	    // widgets are handled when CalcNSetSize::finish() is called.
	    _w->size_available(_w->size());
	}

	void finish() { 
	    if (__dyn_widgets.empty() && __hinted_widgets.empty() ) {
		// There are no dynamically sized widgets, there is nothing
		// left to do
		return;
	    }

	    // In order to process hinted widgets, get the rows. We only consider Widgets hinting on cols
	    Size rows_unhinted(__size.rows()>0 ? __size.rows() : __size_available.rows());
	    assert(rows_unhinted.cols()>0);

	    // There are dynamically sized widgets. So let's find out the how
	    // much space is available for them
	    Size remaining_size(__size_available-__size);

	    HSetSizeHinted hinted_size(std::for_each(__hinted_widgets.begin(),
						     __hinted_widgets.end(),
						     HSetSizeHinted(rows_unhinted)) );

	    remaining_size.cols(remaining_size.cols()-hinted_size.size_used().cols());
	    

	    // We ignore remaining_size.rows() because we horizontally stack
	    // widgets and the dynamically sized widgets get the amount of
	    // __size.rows()
	    assert(remaining_size.cols()>0);

	    // This gives the size for each dynamically sized Widget. The
	    // remaining size will be divided equally among all dynamically
	    // sized widgets.
	    Size per_dyn_widget(__size.rows()>0 ? __size.rows() : __size_available.rows(),
				remaining_size.cols()/__dyn_widgets.size());
				

	    // Set the size for each widget.
	    std::for_each(__dyn_widgets.begin(),
			  __dyn_widgets.end(),
			  HSetSizeAvail(per_dyn_widget));

	}
};

class HSetPosWidget {
    private:
	Coordinates __pos;
    public:
	HSetPosWidget(const Coordinates& __start): __pos(__start){}
	HSetPosWidget(const HSetPosWidget& _o): __pos(_o.__pos){}
	void operator()(WidgetBase* _w) {
	    _w->position(__pos);
	    // Set the size available
	    _w->size_available(_w->size());

	    __pos.x(__pos.x() + _w->size().cols() + 1);
	}
};


//
// Private
//

//
// Protected
//
void
HPack::recalc_size() {
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
    //  HPack* hp1=new HPack();
    //  hp1->add_back(l1); // The pack has not received any size information
    //  hp1->add_back(l2); // ditto
    //
    //  window->widget(hp1); // Now, the Pack receives size_available().
    //
    //  [...]
    //
    if (WidgetBase::size_available() == Size::zero()) return;

    HCalcNSetSize calc = std::for_each(widget_list.begin(),
				       widget_list.end(),
				       HCalcNSetSize(WidgetBase::size_available()));
    
    // This is imperative, in order to set the size_available on any
    // dynamically sized Widgets.
    calc.finish();
}

//
// Public
//
HPack::HPack(): Pack() {
}

HPack::HPack(const HPack& _hp): Pack(_hp) {
}

HPack::~HPack() {}

const HPack&
HPack::operator=(const HPack& _hp) {
    Pack::operator=(_hp);
    return *this;
}

Size
HPack::size_hint() const {
    HCalcSizeHint _size_hint(std::for_each(widget_list.begin(),
					   widget_list.end(),
					   HCalcSizeHint()) );
    assert(_size_hint.hint().cols()==0);
    return _size_hint.hint();
}

void
HPack::realize() {
    if (realized()) throw AlreadyRealized();

    assert(WidgetBase::size_available()!=Size::zero());

    // Set position for each associated widget. That's the only reason we
    // implement realize() in a derived class.
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  HSetPosWidget(position()));

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::realize));

    realized(true);
}
