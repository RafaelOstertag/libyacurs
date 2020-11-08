//
// This file is part of libyacurs.
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

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>

#include "hpack.h"
#include "yacursex.h"

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
    void operator()(WidgetBase* w) {
        assert(w != 0);
        // It is possible, that upon a resize the
        // widget became to big for the screen, or
        // overshoots it due to its position and
        // size. Packs do not check whether or not
        // statically sized become too big when
        // stacked.
        try {
            w->realize();
        } catch (EXCEPTIONS::BaseCurEx&) {
            w->focusgroup_id(FocusManager::nfgid);
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
    Size _size_max;

   public:
    HGetMaxSizeHint() : _size_max(Size::zero()) {}

    HGetMaxSizeHint(const HGetMaxSizeHint& v) : _size_max(v._size_max) {}

    HGetMaxSizeHint& operator=(const HGetMaxSizeHint& v) {
        _size_max = v._size_max;
        return *this;
    }

    void operator()(const WidgetBase* w) {
        assert(w != 0);
        _size_max.cols(w->size_hint().cols() + _size_max.cols());
        _size_max.rows(std::max(w->size_hint().rows(), _size_max.rows()));
    }

    const Size& hint() const { return _size_max; }
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
    const Size& _avail;

   public:
    HSetSizeAvail(const Size& avail) : _avail(avail) {}

    HSetSizeAvail(const HSetSizeAvail& s) : _avail(s._avail) {}

    void operator()(WidgetBase* w) {
        assert(w != 0);
        assert(_avail.rows() > 0);
        assert(_avail.cols() > 0);
        w->size_available(_avail);
    }
};

/**
 * Set size_available on hinted Widgets.
 *
 * Functor for setting size_available on hinted Widgets.
 */
class HSetSizeHinted {
   private:
    Size _const_rows;
    Size _size_used;

   public:
    HSetSizeHinted(const Size& cr) : _const_rows(cr), _size_used() {
        assert(_const_rows.rows() > 0);
    }

    HSetSizeHinted(const HSetSizeHinted& h)
        : _const_rows(h._const_rows), _size_used(h._size_used) {}

    HSetSizeHinted& operator=(const HSetSizeHinted& h) {
        _const_rows = h._const_rows;
        _size_used = h._size_used;
        return *this;
    }

    void operator()(WidgetBase* w) {
        assert(w != 0);
        assert(w->size_hint().cols() > 0);

        // _sa is supposed to hold the constant row
        // value and the hinted cols value as
        // retrieved from calling size_hint() on the
        // Widget
        Size _sa(_const_rows);
        _sa.cols(w->size_hint().cols());

        // Finally, set the available size on the
        // Widget
        w->size_available(_sa);

        // Add the hinted cols to _size_used, so that
        // they can be deducted from the total size
        // available to the pack.
        _size_used.cols(_size_used.cols() + w->size_hint().cols());
    }

    const Size& size_used() const { return _size_used; }
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
    Size _size;
    Size _size_available;
    std::list<WidgetBase*> _dyn_widgets;
    std::list<WidgetBase*> _hinted_widgets;

   public:
    HCalcNSetSize(const Size& av)
        : _size(), _size_available(av), _dyn_widgets(), _hinted_widgets() {
        assert(_size_available.rows() > 0);
        assert(_size_available.cols() > 0);
    }

    HCalcNSetSize(const HCalcNSetSize& c)
        : _size(c._size),
          _size_available(c._size_available),
          _dyn_widgets(c._dyn_widgets),
          _hinted_widgets(c._hinted_widgets) {}

    HCalcNSetSize& operator=(const HCalcNSetSize& c) {
        _size = c._size;
        _size_available = c._size_available;
        _dyn_widgets = c._dyn_widgets;
        _hinted_widgets = c._hinted_widgets;
        return *this;
    }

    void operator()(WidgetBase* w) {
        assert(w != 0);

        // First, reset the size, so that we can
        // identify dynamically sized Widgets
        w->reset_size();

        if (w->size() == Size::zero()) {
            // That's a dynamically sized widget, thus
            // add it to one of the lists for later
            // processing.
            //
            // if it is capable of giving a hint, add
            // it to the _hinted_widgets list which
            // is treated separately from
            // _dyn_widgets. But only if the hint
            // does not exceed the available size.
            if (w->size_hint().cols() > 0 &&
                w->size_hint().cols() <= _size_available.cols())
                _hinted_widgets.push_back(w);
            else
                _dyn_widgets.push_back(w);

            return;
        }

        _size.rows(std::max(_size.rows(), w->size().rows()));
        _size.cols(_size.cols() + w->size().cols());

        if (_size.rows() > _size_available.rows() ||
            _size.cols() > _size_available.cols())
            throw EXCEPTIONS::AreaExceeded();

        // Also set the size availabe for the
        // widget. Dynamically sized widgets are
        // handled when CalcNSetSize::finish() is
        // called.
        w->size_available(w->size());
    }

    void finish() {
        if (_dyn_widgets.empty() && _hinted_widgets.empty()) {
            // There are no dynamically sized widgets,
            // there is nothing left to do
            return;
        }

        // In order to process hinted widgets, get the
        // rows. We only consider Widgets hinting on
        // cols
        Size rows_unhinted(_size_available.rows(), 0);

        HSetSizeHinted hinted_size(
            std::for_each(_hinted_widgets.begin(), _hinted_widgets.end(),
                          HSetSizeHinted(rows_unhinted)));

        // Bail out if there are no unhinted dynamic
        // widgets
        if (_dyn_widgets.empty()) return;

        // There are dynamically sized widgets. So
        // let's find out the how much space is
        // available for them
        Size remaining_size(_size_available - _size);

        remaining_size -= hinted_size.size_used();

        // We ignore remaining_size.rows() because we
        // horizontally stack widgets and the
        // dynamically sized widgets get the amount of
        // _size.rows()
        if (remaining_size.cols() < 1) throw EXCEPTIONS::InvalidSize();

        // This gives the size for each dynamically
        // sized Widget. The remaining size will be
        // divided equally among all dynamically sized
        // widgets.
        Size per_dyn_widget(_size_available.rows(),
                            remaining_size.cols() / _dyn_widgets.size());

        // Set the size for each widget.
        //
        // The following doesn't work when compiling
        // with Solaris Studio on Solaris, so we use a
        // Function Object:
        //
        // std::for_each(_dyn_widgets.begin(),
        //                _dyn_widgets.end(),
        //                std::bind2nd(std::mem_fun<void,WidgetBase,const
        //                Size&>(&WidgetBase::size_available),per_dyn_widget));
        std::for_each(_dyn_widgets.begin(), _dyn_widgets.end(),
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
    Size _size;
    bool _had_dynamic;

   public:
    HCalcSizeNonDynamic() : _size(), _had_dynamic(false) {}

    HCalcSizeNonDynamic(const HCalcSizeNonDynamic& r)
        : _size(r._size), _had_dynamic(r._had_dynamic) {}

    HCalcSizeNonDynamic& operator=(const HCalcSizeNonDynamic& r) {
        _size = r._size;
        _had_dynamic = r._had_dynamic;
        return *this;
    }

    void operator()(const WidgetBase* w) {
        assert(w != 0);

        // Do nothing if we already found a dynamic widget
        if (_had_dynamic) return;

        if (w->size() == Size::zero()) {
            // found a dynamic widget. Reset size and mark
            _had_dynamic = true;
            _size = Size::zero();
            return;
        }

        _size.rows(std::max(_size.rows(), w->size().rows()));
        _size.cols(_size.cols() + w->size().cols());
    }

    const Size& size() const { return _size; }
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
    Coordinates _pos;

   public:
    /**
     * @param start the first call of the object
     * will put the Widget at this position.
     */
    HSetPosWidget(const Coordinates& start) : _pos(start) {}

    HSetPosWidget(const HSetPosWidget& o) : _pos(o._pos) {}

    /**
     * Call WidgetBase::postion().
     *
     * Call WidgetBase::position() and update _pos.
     *
     * @param w pointer to Widget.
     */
    void operator()(WidgetBase* w) {
        w->position(_pos);
        _pos.x(_pos.x() + w->size().cols());
    }
};
}  // namespace HPACK
}  // namespace FUNCTORS
}  // namespace YACURS
//
// Private
//

//
// Protected
//
void HPack::recalc_size() {
    assert(WidgetBase::size_available() != Size::zero());

    FUNCTORS::HPACK::HCalcNSetSize calc(WidgetBase::size_available());
    calc = std::for_each(widget_list.begin(), widget_list.end(), calc);

    // This is imperative, in order to set the size_available on any
    // dynamically sized Widgets.
    calc.finish();
}

Size HPack::calc_size_non_dynamic() const {
    FUNCTORS::HPACK::HCalcSizeNonDynamic s;

    s = std::for_each(widget_list.begin(), widget_list.end(), s);
    return s.size();
}

//
// Public
//
HPack::HPack() : Pack{} {}

HPack::~HPack() {}

Size HPack::size_hint() const {
    if (!hinting()) return Size::zero();

    // remember that Packs may return either component >0 when
    // hinting, see also comment on VGetMaxSizeHint
    FUNCTORS::HPACK::HGetMaxSizeHint shint;

    shint = std::for_each(widget_list.begin(), widget_list.end(), shint);
    return shint.hint();
}

void HPack::realize() {
    REALIZE_ENTER;

    assert(WidgetBase::size_available().rows() > 0);
    assert(WidgetBase::size_available().cols() > 0);

    try {
        recalc_size();
    } catch (EXCEPTIONS::AreaExceeded&) {
        // Back off
        std::for_each(widget_list.begin(), widget_list.end(),[](auto &x) { x->unrealize(); });
        realization(UNREALIZED);
        return;
    }
    // Set position for each associated widget. That's the only reason
    // we implement realize() in a derived class, i.e. here
    std::for_each(widget_list.begin(), widget_list.end(),
                  FUNCTORS::HPACK::HSetPosWidget(position()));

    // We have to catch exceptions, but continue. The
    //
    //    std::for_each(widget_list.begin(),
    //		  widget_list.end(),
    //		  std::mem_fn(&WidgetBase::realize));
    //
    // can't be used for that.
    std::for_each(widget_list.begin(), widget_list.end(),
                  FUNCTORS::HPACK::HRealizeWidgets());

    REALIZE_LEAVE;
}
