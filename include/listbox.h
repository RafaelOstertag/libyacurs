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
#define LISTBOX_H

#include <string>
#include <list>
#include <functional>
#include <cassert>
#include <cstdlib>

#include "widget.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursex.h"
#include "colors.h"

namespace YACURS {
    template<class _T = std::string> class ListBox : public Widget {
        public:
            static bool cmp_asc(const _T& a,
                                const _T& b) {
                return a < b;
            }

            static bool cmp_dsc(const _T& a,
                                const _T& b) {
                return a > b;
            }

            typedef typename std::list<_T>::size_type lsz_t;

            typedef typename _T::size_type tsz_t;
#define __cast_lt(x) (static_cast<lsz_t>(x) )
#define __cast_st(x) (static_cast<tsz_t>(x) )
        private:
            /**
             * Empty value.
             *
             * Used when selected() is called on empty list. This is
             * needed since we return a reference to the item, so we
             * cannot create an empty element in selected() and return a
             * reference to it.
             */
            _T __empty;

            typename std::list<_T> __list;

            /**
             * Offset into the list.
             */
            lsz_t __offset;

            /**
             * Position of the cursor in the visiable area
             */
            lsz_t __curs_pos;

            /**
             * Size of the Widget.
             *
             * May be Size::zero() since dynamically size.
             */
            Size __size;

            /**
             * Sort order of the list.
             *
             * Sort order maintained by the list.
             */
            SORT_ORDER __sort_order;

            // Not supported
            ListBox<_T>& operator=(const ListBox<_T>&) {
                throw EXCEPTIONS::NotSupported();
                return *this;
            }

            inline
            lsz_t pagesize() const {
                return __size.rows() - 2;
            }

        protected:
            virtual void key_handler(Event& _e);

            void sort();

        public:
            /**
             */
            ListBox();

            virtual ~ListBox();

            virtual void add(const _T& _i);

            virtual void set(const std::list<_T>& _l);

            const std::list<_T>& list() const;

            virtual void clear();

            void sort_order(SORT_ORDER _sort_order);

            SORT_ORDER sort_order() const;

            virtual lsz_t selected_index() const;

            const _T& selected() const;

            void selected(_T& _item);

            void delete_selected();

	    /**
	     * High light item.
	     *
	     * high light first item exactly matching @c _i.
	     *
	     * @param _i item to hight light.
	     */
            void high_light(const _T& _i);

	    void high_light(lsz_t pos);

            // From WidgetBase

            void size_available(const Size& _s);

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
             * If __length is zero, resets __size. Else does nothing.
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
    template<class _T> void
    ListBox<_T>::key_handler(Event& _e) {
        assert(_e == EVT_KEY);

        if (!focus() ) return;

#ifdef USE_WCHAR
        EventEx<wint_t>& ekey = dynamic_cast<EventEx<wint_t>&>(_e);
#else
        EventEx<int>& ekey = dynamic_cast<EventEx<int>&>(_e);
#endif

        switch (ekey.data() ) {
        case KEY_ENTER:
        case KEY_RETURN:
        case KEY_RETURN2:
            EventQueue::submit(EventEx<ListBox*>(EVT_LISTBOX_ENTER, this) );
            break;

        case KEY_TAB:
            EventQueue::submit(EVT_FOCUS_NEXT);
            break;

        case KEY_BTAB:
            EventQueue::submit(EVT_FOCUS_PREVIOUS);
            break;

        case KEY_DOWN:
        case KEY_CTRL_N: // Emacs key
        case 'j': // VIM key
            if ( (__offset + __curs_pos) > __list.size() ||
                 __list.empty() ) break;

            // We have to take the box into account, hence
            // __size.rows()-3
            if (__curs_pos < __cast_lt(__size.rows() ) - 3 &&
                __curs_pos < __list.size() - 1) {
                __curs_pos++;
            } else {
                if (__offset + __curs_pos + 1 < __list.size() )
                    __offset++;
            }

            break;

        case KEY_UP:
        case KEY_CTRL_P: // Emacs key
        case 'k': // VIM key
            if ( (__offset == 0 && __curs_pos == 0) ||
                 __list.empty() ) break;

            if (__curs_pos > 0)
                __curs_pos--;
            else
                __offset--;

            break;

        // Solaris' X/Open Curses and System Curses do not
        // support HOME key on PC keyboard, i.e. they tell me
        // it has code `27' which is ESC.
        case KEY_HOME:
        case KEY_CTRL_A:
            __offset = __curs_pos = 0;
            break;

        // Solaris' X/Open Curses and System Curses do not
        // support END key on PC keyboard, i.e. they tell me
        // it has code `27' which is ESC.
        case KEY_END:
        case KEY_CTRL_E:
            if (__list.empty() ) break;

            if (__list.size() <= pagesize() ) {
                __curs_pos = __list.size() - 1;
            } else {
                __offset = __list.size() - __size.rows() + 2;
                __curs_pos = __size.rows() - 3;
            }
            break;

        case KEY_NPAGE:
            if (__list.size() <= pagesize() ||
                __list.empty() ) break;

            if (__cast_lt(__offset +
                          pagesize() ) <= __list.size() - __size.rows() + 2)
                __offset += pagesize();
            else
                __offset = __list.size() - __size.rows() + 2;

            break;

        case KEY_PPAGE:
            if (__list.size() <= pagesize() ||
                __list.empty() ) break;

            if (__offset > pagesize() )
                __offset -= pagesize();
            else
                __offset = 0;
            break;
        }

        refresh(true);
    }

    template<class _T> void
    ListBox<_T>::sort() {
        switch (__sort_order) {
        case ASCENDING:
            __list.sort(ListBox<_T>::cmp_asc);
            break;

        case DESCENDING:
            __list.sort(ListBox<_T>::cmp_dsc);
            break;

        case UNSORTED:
            break;
        }
    }

    //
    // Public
    //
    template<class _T> ListBox<_T>::ListBox() : Widget(),
        __empty(),
        __list(),
        __offset(0),
        __curs_pos(0),
        __size(Size::zero() ),
        __sort_order(UNSORTED) {
        can_focus(true);
    }

    template<class _T> ListBox<_T>::~ListBox() {
        EventQueue::disconnect_event(EventConnectorMethod1<ListBox>(EVT_KEY,
                                                                    this,
                                                                    &ListBox::
                                                                    key_handler) );
    }

    /**
     */
    template<class _T> void
    ListBox<_T>::add(const _T& _i) {
        __list.push_back(_i);

        sort();

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> void
    ListBox<_T>::set(const std::list<_T>& _l) {
        __list = _l;

        sort();

        // Reset these. I don't see a proper way of maintaining
        // these when a completely new `data set' is set.
        __offset = 0;
        __curs_pos = 0;

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> const std::list<_T>&
    ListBox<_T>::list() const {
        return __list;
    }

    template<class _T> void
    ListBox<_T>::clear() {
        __list.clear();
        __offset = 0;
        __curs_pos = 0;

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> void
    ListBox<_T>::sort_order(SORT_ORDER _sort_order) {
        __sort_order = _sort_order;

        sort();

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> SORT_ORDER
    ListBox<_T>::sort_order() const {
        return __sort_order;
    }

    template<class _T> typename ListBox<_T>::lsz_t
    ListBox<_T>::selected_index() const {
        return __curs_pos + __offset;
    }

    template<class _T> const _T&
    ListBox<_T>::selected() const {
        if (__list.empty() ) return __empty;

        typename std::list<_T>::const_iterator it = __list.begin();
        for (lsz_t i = 0; i < __curs_pos + __offset; it++, i++) ;

        return *it;
    }

    template<class _T> void
    ListBox<_T>::selected(_T& _item) {
        typename std::list<_T>::iterator it = __list.begin();
        for (lsz_t i = 0; i < __curs_pos + __offset; it++, i++) ;

        *it = _item;

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> void
    ListBox<_T>::delete_selected() {
        if (__list.empty() ) return;

        typename std::list<_T>::iterator it = __list.begin();
        for (lsz_t i = 0; i < __curs_pos + __offset; it++, i++) ;

        __list.erase(it);

        // Adjust cursor position
        if (__offset > 0) {
            __offset--;
        } else {
            if (__curs_pos > 0)
                __curs_pos--;
        }

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> void
    ListBox<_T>::high_light(const _T& _i) {
        if (__list.empty() ) return;

        typename std::list<_T>::iterator it = __list.begin();
        lsz_t i;
        for (i = 0;
             it != __list.end() && (*it) != _i;
             it++, i++) ;

        if (it == __list.end() ) return;

        if (__list.size() > pagesize() &&
            i > (__list.size() - pagesize() ) ) {
            __offset = __list.size() - pagesize();
            __curs_pos = i - __offset;
        } else {
            __offset = (i / pagesize() ) * pagesize();
            __curs_pos = i - __offset;
        }

        assert(__offset + __curs_pos < __list.size() );

        if (realization() == REALIZED)
            refresh(true);
    }

    template<class _T> void
    ListBox<_T>::high_light(lsz_t pos) {
	if (pos >= __list.length() )
	    throw std::out_of_range("ListBox<>::high_light() position out of range");

        typename std::list<_T>::iterator it = __list.begin();
	lsz_t i;
        for (i = 0;
             it != __list.end();
             it++, i++) ;

        if (it == __list.end() ) return;

        if (__list.size() > pagesize() &&
            i > (__list.size() - pagesize() ) ) {
            __offset = __list.size() - pagesize();
            __curs_pos = i - __offset;
        } else {
            __offset = (i / pagesize() ) * pagesize();
            __curs_pos = i - __offset;
        }

        assert(__offset + __curs_pos < __list.size() );

        if (realization() == REALIZED)
            refresh(true);
    }
    

    // From WidgetBase
    template<class _T> void
    ListBox<_T>::size_available(const Size& _s) {
        WidgetBase::size_available(__size = _s);
    }

    /**
     * Size the Input Widget requires.
     *
     * @return Either Size::zero() if __length is 0, or the
     * Size(1, __length).
     */
    template<class _T> Size
    ListBox<_T>::size() const {
        return __size;
    }

    template<class _T> Size
    ListBox<_T>::size_hint() const {
        return Size::zero();
    }

    /**
     * Dummy. Does nothing.
     *
     * Input Widget is not a container Widget, hence it may not
     * notified of size changes().
     *
     * @return always @false
     */
    template<class _T> bool
    ListBox<_T>::size_change() {
        return false;
    }

    /**
     * Reset size.
     *
     * If __length is zero, resets __size. Else does nothing.
     */
    template<class _T> void
    ListBox<_T>::reset_size() {
        __size = Size::zero();
    }

    // From Realizeable
    template<class _T> void
    ListBox<_T>::realize() {
        REALIZE_ENTER;

        Widget::realize();

        EventQueue::connect_event(EventConnectorMethod1<ListBox>(EVT_KEY,
                                                                 this,
                                                                 &ListBox::
                                                                 key_handler) );

        assert(focusgroup_id() != FocusManager::nfgid);

        REALIZE_LEAVE;
    }

    template<class _T> void
    ListBox<_T>::unrealize() {
        UNREALIZE_ENTER;

        EventQueue::disconnect_event(EventConnectorMethod1<ListBox>(EVT_KEY,
                                                                    this,
                                                                    &ListBox::
                                                                    key_handler) );

        assert(focusgroup_id() != FocusManager::nfgid);

        Widget::unrealize();

        UNREALIZE_LEAVE;
    }

    /**
     * Refresh the Input.
     *
     * @param immediate not directly used by Label::refresh() but
     * passed to Widget::refresh().
     */
    template<class _T> void
    ListBox<_T>::refresh(bool immediate) {
        if (realization() != REALIZED) return;

        assert(widget_subwin() != 0);

        // Set the box. This is only so that initial calls to
        // addlinex() position the text inside the
        // box. Without this call, the first refresh does not
        // know about the border, thus the first character of
        // each line will be overwritten by the border.
        if (!widget_subwin()->has_box() )
            widget_subwin()->box();

        widget_subwin()->set_bg(LISTBOX);

        widget_subwin()->erase();

        std::list<std::string>::iterator it = __list.begin();

        // Make sure cursor position is not off the list, i.e. on
        // the border of the widget.
        __curs_pos = __curs_pos > __cast_lt(__size.rows() ) - 3 ?
                     __size.rows() - 3 :
                     __curs_pos;

        // Make sure the offset will not produce an out of bound,
        // for instance due to a screen resize.
        if (pagesize() > __list.size() ||
            __cast_lt(pagesize() + __offset) > __list.size() )
            __offset = 0; // we must not use an offset.

        // Advance to offset
        for (std::list<std::string>::size_type i = 0;
             i < __offset;
             it++, i++) ;

        for (std::list<std::string>::size_type i = 0;
             i <
             std::min<std::list<std::string>::size_type>(pagesize(),
                                                         __list.size() );
             it++, i++) {
            CurStr line(*it,
                        Coordinates(0,
                                    i + 1),
                        i ==
                        __curs_pos ? YACURS::LISTBOX_HILITE : YACURS::LISTBOX);

            widget_subwin()->addlinex(line);
        }

        // This is required in order to color the listbox properly
        // when highlighting hits the bottom under X/Open Curses
        widget_subwin()->set_color(YACURS::LISTBOX);

        //
        // Box creation and scroll marker setting was moved here,
        // because it didn't work properly, i.e. there were
        // artifcats on the right side of the box, when it is
        // placed in front of the above code block.
        //
        if (focus() ) {
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
        if (__list.size() > pagesize() ) {
            // Can we scroll up? This is indicated by an __offset
            // > 0
            if (__offset > 0)
                widget_subwin()->mvaddch(Coordinates(
                                             __size.cols() - 1, 1), '^');
            // can we scroll further down?
            if (__offset + pagesize() < __list.size() )
                widget_subwin()->mvaddch(Coordinates(__size.cols() - 1,
                                                     __size.rows() - 2), 'v');

        }

        // set sort order indicator
        switch (__sort_order) {
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
        if (focus() && !__list.empty() ) {
            curs_set(1);
            widget_subwin()->leaveok(false);
            widget_subwin()->move(Coordinates(1, __curs_pos + 1) );
        } else {
            curs_set(0);
            widget_subwin()->leaveok(true);
        }

        Widget::refresh(immediate);
    }

#undef __cast_lt
#undef __cast_st
}

#endif // LISTBOX_H
