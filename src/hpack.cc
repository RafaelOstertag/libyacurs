//
// This file is part of libyacurs, 
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <functional>

#include "yacursex.h"
#include "hpack.h"

using namespace YACURS;

//
// Functors
//

namespace YACURS {
    namespace FUNCTORS {
	namespace HPACK {
	    /**
	     * Realizes widgets.
	     *
	     * It checks for exceptions when realizing.
	     */
	    class HRealizeWidgets {
		public:
		    void operator()(WidgetBase* _w) {
			assert(_w!=0);
			// It is possible, that upon a resize the
			// widget became to big for the screen, or
			// overshoots it due to its position and
			// size. Packs do not check whether or not
			// statically sized become too big when
			// stacked.
			try {
			    _w->realize();
			} catch (EXCEPTIONS::BaseCurEx&) {
			} 
		    }
	    };

	    /**
	     * Calculate the size hint.
	     *
	     * Functor for calculating the size hint by finding the
	     * max row size and summing up column size.
	     */
	    class HGetMaxSizeHint {
		private:
		    Size __size_max;

		public:
		    HGetMaxSizeHint(): __size_max(Size::zero()) {}
		    HGetMaxSizeHint(const HGetMaxSizeHint& _v):
			__size_max(_v.__size_max) {}
		    HGetMaxSizeHint& operator=(const HGetMaxSizeHint& _v) {
			__size_max=_v.__size_max;
			return *this;
		    }

		    void operator()(const WidgetBase* w) {
			assert(w!=0);
			__size_max.cols(w->size_hint().cols()+__size_max.cols());
			__size_max.rows(std::max(w->size_hint().rows(),
						 __size_max.rows()));
		    }

		    const Size& hint() const {
			return __size_max;
		    }
	    };

	    /**
	     * Set size_available() on widgets.
	     *
	     * Functor for setting available size on Widgets.
	     *
	     * @internal
	     *
	     * mainly used when compiling with Solaris Studio.
	     */
	    class HSetSizeAvail {
		private:
		    const Size& __avail;

		public:
		    HSetSizeAvail(const Size& _avail): __avail(_avail) {}
		    HSetSizeAvail(const HSetSizeAvail& _s): __avail(_s.__avail) {}
		    void operator()(WidgetBase* w) {
			assert(w!=0);
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
	    class HSetSizeHinted {
		private:
		    Size __const_rows;
		    Size __size_used;

		public:
		    HSetSizeHinted(const Size& _cr): __const_rows(_cr),
						     __size_used() {
			assert(__const_rows.rows()>0);
		    }

		    HSetSizeHinted(const HSetSizeHinted& _h):
			__const_rows(_h.__const_rows),
			__size_used(_h.__size_used) {}

		    HSetSizeHinted& operator=(const HSetSizeHinted& _h) {
			__const_rows=_h.__const_rows;
			__size_used=_h.__size_used;
			return *this;
		    }

		    void operator()(WidgetBase* _w) {
			assert(_w!=0);
			assert(_w->size_hint().cols()>0);

			// _sa is supposed to hold the constant row
			// value and the hinted cols value as
			// retrieved from calling size_hint() on the
			// Widget
			Size _sa(__const_rows);
			_sa.cols(_w->size_hint().cols());

			// Finally, set the available size on the
			// Widget
			_w->size_available(_sa);

			// Add the hinted cols to __size_used, so that
			// they can be deducted from the total size
			// available to the pack.
			__size_used.cols(__size_used.cols()+_w->size_hint().cols());
		    }

		    const Size& size_used() const {
			return __size_used;
		    }
	    };

	    /**
	     * Calculates and sets the size required by associated
	     * Widgets.
	     *
	     * This class is passed in a std::for_each(). Once
	     * for_each() is done, finish() must be called, since that
	     * will set size on all dynamically sized Widgets.
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
						    __hinted_widgets() {
			assert(__size_available.rows()>0);
			assert(__size_available.cols()>0);
		    }

		    HCalcNSetSize(const HCalcNSetSize& _c):
			__size(_c.__size),
			__size_available(_c.__size_available),
			__dyn_widgets(_c.__dyn_widgets),
			__hinted_widgets(_c.__hinted_widgets) {}

		    HCalcNSetSize& operator=(const HCalcNSetSize& _c) {
			__size = _c.__size;
			__size_available = _c.__size_available;
			__dyn_widgets = _c.__dyn_widgets;
			__hinted_widgets = _c.__hinted_widgets;
			return *this;
		    }

		    void operator()(WidgetBase* _w) {
			assert(_w != 0);

			// First, reset the size, so that we can
			// identify dynamically sized Widgets
			_w->reset_size();

			if ( _w->size() == Size::zero() ) {
			    // That's a dynamically sized widget, thus
			    // add it to one of the lists for later
			    // processing.
			    //
			    // if it is capable of giving a hint, add
			    // it to the __hinted_widgets list which
			    // is treated separately from
			    // __dyn_widgets. But only if the hint
			    // does not exceed the available size.
			    if (_w->size_hint().cols()>0 &&
				_w->size_hint().cols()<=__size_available.cols())
				__hinted_widgets.push_back(_w);
			    else
				__dyn_widgets.push_back(_w);

			    return;
			}

			__size.rows(std::max(__size.rows(),_w->size().rows()) );
			__size.cols(__size.cols()+_w->size().cols());

			if (__size.rows()>__size_available.rows() ||
			    __size.cols()>__size_available.cols())
			    throw EXCEPTIONS::AreaExceeded();

			// Also set the size availabe for the
			// widget. Dynamically sized widgets are
			// handled when CalcNSetSize::finish() is
			// called.
			_w->size_available(_w->size());
		    }

		    void finish() {
			if (__dyn_widgets.empty() && __hinted_widgets.empty() ) {
			    // There are no dynamically sized widgets,
			    // there is nothing left to do
			    return;
			}

			// In order to process hinted widgets, get the
			// rows. We only consider Widgets hinting on
			// cols
			Size rows_unhinted(__size_available.rows(), 0);

			HSetSizeHinted hinted_size(std::for_each(__hinted_widgets.begin(),
								 __hinted_widgets.end(),
								 HSetSizeHinted(rows_unhinted)) );

			// Bail out if there are no unhinted dynamic
			// widgets
			if (__dyn_widgets.empty()) return;

			// There are dynamically sized widgets. So
			// let's find out the how much space is
			// available for them
			Size remaining_size(__size_available-__size);

			remaining_size-=hinted_size.size_used();

			// We ignore remaining_size.rows() because we
			// horizontally stack widgets and the
			// dynamically sized widgets get the amount of
			// __size.rows()
			assert(remaining_size.cols()>0);

			// This gives the size for each dynamically
			// sized Widget. The remaining size will be
			// divided equally among all dynamically sized
			// widgets.
			Size per_dyn_widget(__size_available.rows(),
					    remaining_size.cols()/__dyn_widgets.size());

			// Set the size for each widget.
			//
			// The following doesn't work when compiling
			// with Solaris Studio on Solaris, so we use a
			// Function Object:
			//
			// std::for_each(__dyn_widgets.begin(),
			// 		  __dyn_widgets.end(),
			// 		  std::bind2nd(std::mem_fun<void,WidgetBase,const Size&>(&WidgetBase::size_available),per_dyn_widget));
			std::for_each(__dyn_widgets.begin(),
				      __dyn_widgets.end(),
				      HSetSizeAvail(per_dyn_widget));
		    }
	    };

	    /**
	     * Functor calculating the size only if no dynamically
	     * sized Widgets are associated. Else it will return
	     * Size::zero().
	     *
	     * Used in calc_size_non_dynamic().
	     */
	    class HCalcSizeNonDynamic {
		private:
		    Size __size;
		    bool __had_dynamic;
		public:
		    HCalcSizeNonDynamic(): __size(), __had_dynamic(false) {}
		    HCalcSizeNonDynamic(const HCalcSizeNonDynamic& _r):
			__size(_r.__size),
			__had_dynamic(_r.__had_dynamic){}
		    HCalcSizeNonDynamic& operator=(const HCalcSizeNonDynamic& _r) {
			__size = _r.__size;
			__had_dynamic = _r.__had_dynamic;
			return *this;
		    }
		    void operator()(const WidgetBase* _w) {
			assert(_w!=0);

			// Do nothing if we already found a dynamic widget
			if (__had_dynamic) return;

			if (_w->size() == Size::zero() ) {
			    // found a dynamic widget. Reset size and mark
			    __had_dynamic = true;
			    __size = Size::zero();
			    return;
			}

			__size.rows(std::max(__size.rows(),_w->size().rows()));
			__size.cols(__size.cols()+_w->size().cols());
		    }
		    const Size& size() const {
			return __size;
		    }
	    };

	    /**
	     * Sets the position of a given Widget.
	     *
	     * Sets the position of a given Widget and updates an
	     * internal Coordinate object in manner, that subsequent
	     * positions will not overwrite previous Widgets on the
	     * screen.
	     *
	     * The position will be advanced from left to right
	     * (i.e. horizontally).
	     */
	    class HSetPosWidget {
		private:
		    /**
		     * Position of wich is used in the call to
		     * WidgetBase::position().
		     */
		    Coordinates __pos;

		public:
		    /**
		     * @param __start the first call of the object
		     * will put the Widget at this position.
		     */
		    HSetPosWidget(const Coordinates& __start): __pos(__start){}

		    HSetPosWidget(const HSetPosWidget& _o): __pos(_o.__pos){}

		    /**
		     * Call WidgetBase::postion().
		     *
		     * Call WidgetBase::position() and update __pos.
		     *
		     * @param _w pointer to Widget.
		     */
		    void operator()(WidgetBase* _w) {
			_w->position(__pos);
			__pos.x(__pos.x() + _w->size().cols());
		    }
	    };
	} // namespace HPACK
    } // namespace FUNCTORS
} // namespace YACURS
//
// Private
//

HPack&
HPack::operator=(const HPack&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}
//
// Protected
//
void
HPack::recalc_size() {
    assert(WidgetBase::size_available() != Size::zero());

    FUNCTORS::HPACK::HCalcNSetSize calc(WidgetBase::size_available());
    calc = std::for_each(widget_list.begin(),
			 widget_list.end(),
			 calc);

    // This is imperative, in order to set the size_available on any
    // dynamically sized Widgets.
    calc.finish();
}

Size
HPack::calc_size_non_dynamic() const {
    FUNCTORS::HPACK::HCalcSizeNonDynamic _s;
    _s=std::for_each(widget_list.begin(),
		     widget_list.end(),
		     _s);
    return _s.size();
}

//
// Public
//
HPack::~HPack() {}

Size
HPack::size_hint() const {
    if (!hinting()) return Size::zero();

    // remember that Packs may return either component >0 when
    // hinting, see also comment on VGetMaxSizeHint
    FUNCTORS::HPACK::HGetMaxSizeHint shint;

    shint = std::for_each(widget_list.begin(),
			  widget_list.end(),
			  shint);
    return shint.hint();
}

void
HPack::realize() {
    REALIZE_ENTER;

    assert(WidgetBase::size_available().rows()>0);
    assert(WidgetBase::size_available().cols()>0);

    try {
	recalc_size();
    } catch (EXCEPTIONS::AreaExceeded&) {
	// Back off
	std::for_each(widget_list.begin(),
		  widget_list.end(),
		  std::mem_fun(&WidgetBase::unrealize));
	realization(UNREALIZED);
	return;
    }
    // Set position for each associated widget. That's the only reason
    // we implement realize() in a derived class, i.e. here
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  FUNCTORS::HPACK::HSetPosWidget(position()));

    // We have to catch exceptions, but continue. The 
    //
    //    std::for_each(widget_list.begin(),
    //		  widget_list.end(),
    //		  std::mem_fun(&WidgetBase::realize));
    //
    // can't be used for that.
    std::for_each(widget_list.begin(),
		  widget_list.end(),
		  FUNCTORS::HPACK::HRealizeWidgets());

    REALIZE_LEAVE;
}
