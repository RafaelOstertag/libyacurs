// -*- mode: c++ -*-
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

#ifndef LISTBOX_H
#define LISTBOX_H 1

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <list>
#include <string>

#include "colors.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "widget.h"
#include "yacursex.h"

namespace YACURS {

template <class T = std::string>
class ListBox : public Widget {
   public:
    static bool cmp_asc(const T& a, const T& b) { return a < b; }

    static bool cmp_dsc(const T& a, const T& b) { return a > b; }

    typedef typename std::list<T>::size_type lsz_t;

    typedef typename T::size_type tsz_t;
#define __cast_lt(x) (static_cast<lsz_t>(x))
#define __cast_st(x) (static_cast<tsz_t>(x))
   private:
    /**
     * Empty value.
     *
     * Used when selected() is called on empty list. This is
     * needed since we return a reference to the item, so we
     * cannot create an empty element in selected() and return a
     * reference to it.
     */
    T _empty;

    typename std::list<T> _list;

    /**
     * Offset into the list.
     */
    lsz_t _offset;

    /**
     * Position of the cursor in the visiable area
     */
    lsz_t _curs_pos;

    /**
     * Size of the Widget.
     *
     * May be Size::zero() since dynamically size.
     */
    Size _size;

    /**
     * Sort order of the list.
     *
     * Sort order maintained by the list.
     */
    SORT_ORDER _sort_order;

    lsz_t pagesize() const { return _size.rows() - 2; }

   protected:
    virtual void key_handler(Event& e);

    void sort();

   public:
    /**
     */
    ListBox();

    ListBox(ListBox&&) = delete;
    ListBox(const ListBox&) = delete;
    ListBox& operator=(ListBox&&) = delete;
    ListBox& operator=(const ListBox&) = delete;
    virtual ~ListBox();

    void add(const T& i);

    void set(const std::list<T>& l);

    /**
     * Replace item.
     *
     * Replace the item @c item at position @c index.
     *
     * If @c index is greater than the list size, a @c
     * std::out_of_range exception is thrown.
     *
     * @param item new value
     *
     * @param index position where to set the value.
     */
    virtual void set(const T& item, lsz_t index);

    const std::list<T>& list() const;

    bool empty() const;

    lsz_t count() const;

    void clear();

    void sort_order(SORT_ORDER sort_order);

    SORT_ORDER sort_order() const;

    lsz_t selected_index() const;

    const T& selected() const;

    /**
     * Replace the selected item.
     *
     * Replaced he selected item by @c item
     *
     * @param item new value of selected item.
     */
    void selected(T& item);

    /**
     * Delete selected item.
     *
     * Delete selected item.
     *
     * @todo when deleting, don't move cursor upwards unless
     * deleteing last item in list.
     */
    void delete_selected();

    void high_light(lsz_t pos);

    template <class _Pred>
    bool search(_Pred p, lsz_t start = 0, lsz_t* pos = 0);

    // From WidgetBase

    void size_available(const Size& s);

    /**
     * Size the Input Widget requires.
     *
     * @return Either Size::zero() if __length is 0, or the
     * Size(1, __length).
     */
    Size size() const;

    Size size_hint() const;

    /**
     * Dummy. Does nothing.
     *
     * Input Widget is not a container Widget, hence it may not
     * notified of size changes().
     *
     * @return always @false
     */
    bool size_change();

    /**
     * Reset size.
     *
     * If __length is zero, resets _size. Else does nothing.
     */
    void reset_size();

    // From Realizeable
    void realize();

    void unrealize();

    /**
     * Refresh the Input.
     *
     * @param immediate not directly used by Label::refresh() but
     * passed to Widget::refresh().
     */
    void refresh(bool immediate);
};

//
// Protected
//
template <class T>
void ListBox<T>::key_handler(Event& e) {
    assert(e == EVT_KEY);

    if (!focus()) return;

#ifdef YACURS_USE_WCHAR
    EventEx<wint_t>& ekey = dynamic_cast<EventEx<wint_t>&>(e);
#else
    EventEx<int>& ekey = dynamic_cast<EventEx<int>&>(e);
#endif

    switch (ekey.data()) {
        case KEY_ENTER:
        case KEY_RETURN:
        case KEY_RETURN2:
            EventQueue::submit(EventEx<ListBox<T>*>(EVT_LISTBOX_ENTER, this));
            break;

        case KEY_TAB:
            EventQueue::submit(EVT_FOCUS_NEXT);
            break;

        case KEY_BTAB:
            EventQueue::submit(EVT_FOCUS_PREVIOUS);
            break;

        case KEY_DOWN:
        case KEY_CTRL_N:  // Emacs key
        case 'j':         // VIM key
            if ((_offset + _curs_pos) > _list.size() || _list.empty()) break;

            // We have to take the box into account, hence
            // _size.rows()-3
            if (_curs_pos < __cast_lt(_size.rows()) - 3 &&
                _curs_pos < _list.size() - 1) {
                _curs_pos++;
            } else {
                if (_offset + _curs_pos + 1 < _list.size()) _offset++;
            }

            break;

        case KEY_UP:
        case KEY_CTRL_P:  // Emacs key
        case 'k':         // VIM key
            if ((_offset == 0 && _curs_pos == 0) || _list.empty()) break;

            if (_curs_pos > 0)
                _curs_pos--;
            else
                _offset--;

            break;

        // Solaris' X/Open Curses and System Curses do not
        // support HOME key on PC keyboard, i.e. they tell me
        // it has code `27' which is ESC.
        case KEY_HOME:
        case KEY_CTRL_A:
            _offset = _curs_pos = 0;
            break;

        // Solaris' X/Open Curses and System Curses do not
        // support END key on PC keyboard, i.e. they tell me
        // it has code `27' which is ESC.
        case KEY_END:
        case KEY_CTRL_E:
            if (_list.empty()) break;

            if (_list.size() <= pagesize()) {
                _curs_pos = _list.size() - 1;
            } else {
                _offset = _list.size() - _size.rows() + 2;
                _curs_pos = _size.rows() - 3;
            }
            break;

        case KEY_NPAGE:
            if (_list.size() <= pagesize() || _list.empty()) break;

            if (__cast_lt(_offset + pagesize()) <=
                _list.size() - _size.rows() + 2)
                _offset += pagesize();
            else
                _offset = _list.size() - _size.rows() + 2;

            break;

        case KEY_PPAGE:
            if (_list.size() <= pagesize() || _list.empty()) break;

            if (_offset > pagesize())
                _offset -= pagesize();
            else
                _offset = 0;
            break;
    }

    refresh(true);
}

template <class T>
void ListBox<T>::sort() {
    switch (_sort_order) {
        case ASCENDING:
            _list.sort(ListBox<T>::cmp_asc);
            break;

        case DESCENDING:
            _list.sort(ListBox<T>::cmp_dsc);
            break;

        case UNSORTED:
            break;
    }
}

//
// Public
//
template <class T>
ListBox<T>::ListBox()
    : Widget(),
      _empty(),
      _list(),
      _offset(0),
      _curs_pos(0),
      _size(Size::zero()),
      _sort_order(UNSORTED) {
    can_focus(true);
}

template <class T>
ListBox<T>::~ListBox() {
    EventQueue::disconnect_event(
        EventConnectorMethod1<ListBox>(EVT_KEY, this, &ListBox::key_handler));
}

/**
 */
template <class T>
void ListBox<T>::add(const T& i) {
    _list.push_back(i);

    sort();

    if (realization() == REALIZED) refresh(true);
}

template <class T>
void ListBox<T>::set(const std::list<T>& l) {
    _list = l;

    sort();

    // Reset these. I don't see a proper way of maintaining
    // these when a completely new `data set' is set.
    _offset = 0;
    _curs_pos = 0;

    if (realization() == REALIZED) refresh(true);
}

template <class T>
void ListBox<T>::set(const T& item, lsz_t index) {
    if (index >= _list.size())
        throw std::out_of_range("ListBox<>::set() position out of range");

    typename std::list<T>::iterator it = _list.begin();
    std::advance(it, index);

    *it = item;

    sort();
}

template <class T>
const std::list<T>& ListBox<T>::list() const {
    return _list;
}

template <class T>
typename ListBox<T>::lsz_t ListBox<T>::count() const {
    return _list.size();
}

template <class T>
bool ListBox<T>::empty() const {
    return _list.empty();
}

template <class T>
void ListBox<T>::clear() {
    _list.clear();
    _offset = 0;
    _curs_pos = 0;

    if (realization() == REALIZED) refresh(true);
}

template <class T>
void ListBox<T>::sort_order(SORT_ORDER sort_order) {
    _sort_order = sort_order;

    sort();

    if (realization() == REALIZED) refresh(true);
}

template <class T>
SORT_ORDER ListBox<T>::sort_order() const {
    return _sort_order;
}

template <class T>
typename ListBox<T>::lsz_t ListBox<T>::selected_index() const {
    return _curs_pos + _offset;
}

template <class T>
const T& ListBox<T>::selected() const {
    if (_list.empty()) return _empty;

    assert((_curs_pos + _offset) < _list.size());

    typename std::list<T>::const_iterator it = _list.begin();
    std::advance(it, _curs_pos + _offset);

    return *it;
}

template <class T>
void ListBox<T>::selected(T& item) {
    assert((_curs_pos + _offset) < _list.size());

    typename std::list<T>::iterator it = _list.begin();
    std::advance(it, _curs_pos + _offset);

    *it = item;

    sort();

    if (realization() == REALIZED) refresh(true);
}

template <class T>
void ListBox<T>::delete_selected() {
    if (_list.empty()) return;

    assert((_curs_pos + _offset) < _list.size());

    typename std::list<T>::iterator it = _list.begin();
    std::advance(it, _curs_pos + _offset);

    _list.erase(it);

    // Adjust cursor position
    if (_offset > 0) {
        _offset--;
    } else {
        if (_curs_pos > 0) _curs_pos--;
    }

    if (realization() == REALIZED) refresh(true);
}

template <class T>
void ListBox<T>::high_light(lsz_t pos) {
    if (pos >= _list.size())
        throw std::out_of_range(
            "ListBox<>::high_light() position out of range");

    typename std::list<T>::iterator it = _list.begin();
    std::advance(it, pos);

    if (_list.size() > pagesize() && pos > (_list.size() - pagesize())) {
        _offset = _list.size() - pagesize();
        _curs_pos = pos - _offset;
    } else {
        _offset = (pos / pagesize()) * pagesize();
        _curs_pos = pos - _offset;
    }

    assert(_offset + _curs_pos < _list.size());

    if (realization() == REALIZED) refresh(true);
}

template <class T>
template <class _Pred>
bool ListBox<T>::search(_Pred p, lsz_t start, lsz_t* pos) {
    if (start >= _list.size()) return false;

    typename std::list<T>::iterator start_it = _list.begin();

    std::advance(start_it, start);

    typename std::list<T>::iterator it = std::find_if(start_it, _list.end(), p);
    // see if we have a match
    if (it != _list.end()) {
        typename std::list<T>::difference_type dist = 0;
#if defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x5150)
        std::distance(_list.begin(), it, dist);
#else
        dist = std::distance(_list.begin(), it);
#endif

        if (pos != 0) *pos = dist;

        high_light(dist);

        return true;
    }

    return false;
}

// From WidgetBase
template <class T>
void ListBox<T>::size_available(const Size& s) {
    WidgetBase::size_available(_size = s);
}

/**
 * Size the Input Widget requires.
 *
 * @return Either Size::zero() if __length is 0, or the
 * Size(1, __length).
 */
template <class T>
Size ListBox<T>::size() const {
    return _size;
}

template <class T>
Size ListBox<T>::size_hint() const {
    return Size::zero();
}

/**
 * Dummy. Does nothing.
 *
 * Input Widget is not a container Widget, hence it may not
 * notified of size changes().
 *
 * @return always @c false
 */
template <class T>
bool ListBox<T>::size_change() {
    return false;
}

/**
 * Reset size.
 */
template <class T>
void ListBox<T>::reset_size() {
    _size = Size::zero();
}

// From Realizeable
template <class T>
void ListBox<T>::realize() {
    REALIZE_ENTER;

    Widget::realize();

    EventQueue::connect_event(
        EventConnectorMethod1<ListBox>(EVT_KEY, this, &ListBox::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    REALIZE_LEAVE;
}

template <class T>
void ListBox<T>::unrealize() {
    UNREALIZE_ENTER;

    EventQueue::disconnect_event(
        EventConnectorMethod1<ListBox>(EVT_KEY, this, &ListBox::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    Widget::unrealize();

    UNREALIZE_LEAVE;
}

template <class T>
void ListBox<T>::refresh(bool immediate) {
    if (realization() != REALIZED) return;

    assert(widget_subwin() != 0);

    // Set the box. This is only so that initial calls to
    // addlinex() position the text inside the
    // box. Without this call, the first refresh does not
    // know about the border, thus the first character of
    // each line will be overwritten by the border.
    if (!widget_subwin()->has_box()) widget_subwin()->box();

    widget_subwin()->set_bg(LISTBOX);

    widget_subwin()->erase();

    typename std::list<T>::iterator it = _list.begin();

    // Make sure cursor position is not off the list, i.e. on
    // the border of the widget.
    _curs_pos =
        _curs_pos > __cast_lt(_size.rows()) - 3 ? _size.rows() - 3 : _curs_pos;

    // Make sure the offset will not produce an out of bound,
    // for instance due to a screen resize.
    if (pagesize() > _list.size() ||
        __cast_lt(pagesize() + _offset) > _list.size())
        _offset = 0;  // we must not use an offset.

    // Advance to offset
    std::advance(it, _offset);

    for (typename std::list<T>::size_type i = 0;
         i <
         std::min<typename std::list<T>::size_type>(pagesize(), _list.size());
         it++, i++) {
        CurStr line(*it, Coordinates(0, i + 1),
                    i == _curs_pos ? YACURS::LISTBOX_HILITE : YACURS::LISTBOX);

        widget_subwin()->addlinex(line);
    }

    // This is required in order to color the listbox properly
    // when highlighting hits the bottom under X/Open Curses
    widget_subwin()->set_color(YACURS::LISTBOX);

    //
    // Box creation and scroll marker setting was moved here,
    // because it didn't work properly, i.e. there were
    // artifacts on the right side of the box, when it is
    // placed in front of the above code block.
    //
    if (focus()) {
        widget_subwin()->box();
    } else {
        // See CursWin::box() about #ifdef
#if !defined(_XOPEN_CURSES) || defined(NCURSES_VERSION)
        widget_subwin()->box('|', '-');
#else
        widget_subwin()->box('.', '.');
#endif
    }

    // set scroll markers
    if (_list.size() > pagesize()) {
        // Can we scroll up? This is indicated by an _offset
        // > 0
        if (_offset > 0)
            widget_subwin()->mvaddch(Coordinates(_size.cols() - 1, 1), '^');
        // can we scroll further down?
        if (_offset + pagesize() < _list.size())
            widget_subwin()->mvaddch(
                Coordinates(_size.cols() - 1, _size.rows() - 2), 'v');
    }

    // set sort order indicator
    switch (_sort_order) {
        case ASCENDING:
            widget_subwin()->mvaddch(Coordinates(1, 0), '^');
            break;

        case DESCENDING:
            widget_subwin()->mvaddch(Coordinates(1, 0), 'v');
            break;

        case UNSORTED:
            break;
    }

    // Set the cursor at the right position if we have focus.
    if (focus() && !_list.empty()) {
        curs_set(1);
        widget_subwin()->leaveok(false);
        widget_subwin()->move(Coordinates(1, _curs_pos + 1));
    } else {
        curs_set(0);
        widget_subwin()->leaveok(true);
    }

    Widget::refresh(immediate);
}

#undef __cast_lt
#undef __cast_st
}  // namespace YACURS

#endif  // LISTBOX_H
