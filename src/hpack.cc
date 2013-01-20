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

    public:
	HCalcNSetSize(const Size& _av): __size(),
				       __size_available(_av),
				       __dyn_widgets() {}

	HCalcNSetSize(const HCalcNSetSize& _c): __size(_c.__size),
						__size_available(_c.__size_available),
						__dyn_widgets(_c.__dyn_widgets) {}

	HCalcNSetSize& operator=(const HCalcNSetSize& _c) {
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


	    __size.rows(__size.rows() < _w->size().rows() ?
			_w->size().rows() : __size.rows());
	    __size.cols(__size.cols()+_w->size().cols());

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

	    // We ignore remaining_size.rows() because we vertically stack
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

class HSetPosAssoc {
    private:
	Coordinates __pos;
    public:
	HSetPosAssoc(const Coordinates& __start): __pos(__start){}
	HSetPosAssoc(const HSetPosAssoc& _o): __pos(_o.__pos){}
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

void
HPack::realize() {
    if (realized()) throw AlreadyRealized();

    assert(WidgetBase::size_available()!=Size::zero());

    // Set position for each associated widget. That's the only reason we
    // implement realize() in a derived class.
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  HSetPosAssoc(position()));

    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::realize));

    realized(true);
}
