// -*- mode: c++ -*-
//
// $Id$

#ifndef LISTBOX_H
#define LISTBOX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

/**
 *
 */
template<class _T=std::string>
class ListBox: public Widget {
    public:
	enum SORT_ORDER {
	    ASCENDING,
	    DESCENDING,
	    UNSORTED
	};

	static bool cmp_asc(const _T& a,
			    const _T& b) {
	    return a<b;
	}

	static bool cmp_dsc(const _T& a,
			    const _T& b) {
	    return a>b;
	}

	typedef typename std::list<_T>::size_type lsz_t;
	typedef typename _T::size_type tsz_t;
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
	    throw NotSupported();
	    return *this;
	}

    protected:
	virtual void key_handler(Event& _e) {
	    assert(_e==EVT_KEY);

	    if (!focus()) return;

	    EventEx<int>& ekey=dynamic_cast<EventEx<int>&>(_e);

	    switch (ekey.data()) {
	    case KEY_ENTER:
	    case KEY_RETURN:
	    case KEY_RETURN2:
		EventQueue::submit(EventEx<ListBox*>(EVT_LISTBOX_ENTER,this));
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
		if ((__offset+__curs_pos)>__list.size() ||
		    __list.empty()) break;

		// We have to take the box into account, hence
		// __size.rows()-3
		if (__curs_pos<__cast_lt(__size.rows())-3 &&
		    __curs_pos<__list.size()-1) {
		    __curs_pos++;
		} else {
		    if (__offset+__curs_pos+1 < __list.size())
			__offset++;
		}

		break;
	    case KEY_UP:
	    case KEY_CTRL_P: // Emacs key
	    case 'k': // VIM key
		if ((__offset==0 && __curs_pos==0) ||
		    __list.empty()) break;

		if (__curs_pos>0)
		    __curs_pos--;
		else
		    __offset--;

		break;
		// Solaris' X/Open Curses and System Curses do not
		// support HOME key on PC keyboard, i.e. they tell me
		// it has code `27' which is ESC.
	    case KEY_HOME:
	    case KEY_CTRL_A:
		__offset=__curs_pos=0;
		break;
		// Solaris' X/Open Curses and System Curses do not
		// support END key on PC keyboard, i.e. they tell me
		// it has code `27' which is ESC.
	    case KEY_END:
	    case KEY_CTRL_E:
		if (__list.empty()) break;

		if (__list.size()<=__cast_lt(__size.rows())-2) {
		    __curs_pos=__list.size()-1;
		} else {
		    __offset=__list.size()-__size.rows()+2;
		    __curs_pos=__size.rows()-3;
		}
		break;
	    case KEY_NPAGE:
		if (__list.size()<=__cast_lt(__size.rows())-2 ||
		    __list.empty()) break;

		if (__cast_lt(__offset+__size.rows())-2<=__list.size()-__size.rows()+2)
		    __offset+=__size.rows()-2;
		else
		    __offset=__list.size()-__size.rows()+2;

		break;
	    case KEY_PPAGE:
		if (__list.size()<=__cast_lt(__size.rows())-2 ||
		    __list.empty()) break;

		if (__offset>__cast_lt(__size.rows())-2)
		    __offset-=__size.rows()-2;
		else
		    __offset=0;
		break;
	    case 'o':
	    case 'O':
		// Change sort order
		switch (__sort_order) {
		case ASCENDING:
		    __sort_order=DESCENDING;
		    break;
		case DESCENDING:
		    __sort_order=ASCENDING;
		    break;
		case UNSORTED:
		    __sort_order=ASCENDING;
		    break;
		}
		sort();
		break;
	    }

	    refresh(true);
	}

	// From Realizeable
	void realize() {
	    REALIZE_ENTER;

	    Widget::realize();

	    EventQueue::connect_event(EventConnectorMethod1<ListBox>(EVT_KEY,this, &ListBox::key_handler));

	    assert(focusgroup_id()!=(fgid_t)-1);

	    REALIZE_LEAVE;
	}

	void unrealize() {
	    UNREALIZE_ENTER;

	    EventQueue::disconnect_event(EventConnectorMethod1<ListBox>(EVT_KEY,this, &ListBox::key_handler));

	    assert(focusgroup_id()!=(fgid_t)-1);

	    Widget::unrealize();

	    UNREALIZE_LEAVE;
	}

	void sort() {
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

    public:
	/**
	 */
	ListBox(): Widget(),
		   __empty(),
		   __list(),
		   __offset(0),
		   __curs_pos(0),
		   __size(Size::zero()),
		   __sort_order(UNSORTED) {
	    can_focus(true);
	}

	virtual ~ListBox() {
	    EventQueue::disconnect_event(EventConnectorMethod1<ListBox>(EVT_KEY, this, &ListBox::key_handler));
	}

	/**
	 */
	virtual void add(const _T& _i) {
	    __list.push_back(_i);

	    sort();

	    if (realization()==REALIZED)
		refresh(true);
	}

	virtual void set(const std::list<_T>& _l) {
	    __list=_l;

	    sort();

	    // Reset these. I don't see a proper way of maintaining
	    // these when a completely new `data set' is set.
	    __offset=0;
	    __curs_pos=0;

	    if (realization()==REALIZED)
		refresh(true);
	}

	const std::list<_T>& list() const {
	    return __list;
	}

	virtual void clear()  {
	    __list.clear();
	    __offset=0;
	    __curs_pos=0;

	    if (realization()==REALIZED) 
		refresh(true);
	}

	void sort_order(SORT_ORDER _sort_order) {
	    __sort_order=_sort_order;

	    sort();

	    if (realization()==REALIZED)
		refresh(true);
	}

	SORT_ORDER sort_order() const {
	    return __sort_order;
	}

	virtual lsz_t selected_index() const {
	    return __curs_pos + __offset;
	}

	const _T& selected() const {
	    if (__list.empty()) return __empty;

	    typename std::list<_T>::const_iterator it=__list.begin();
	    for(lsz_t i=0; i<__curs_pos+__offset; it++, i++);

	    return *it;
	}

	void selected(_T& _item) {
	    typename std::list<_T>::iterator it=__list.begin();
	    for(lsz_t i=0; i<__curs_pos+__offset; it++, i++);

	    *it=_item;

	    if (realization()==REALIZED)
		refresh(true);
	}

	void delete_selected() {
	    if (__list.empty()) return;

	    typename std::list<_T>::iterator it=__list.begin();
	    for(lsz_t i=0;i<__curs_pos+__offset; it++, i++);

	    __list.erase(it);

	    // Adjust cursor position
	    if (__offset>0) {
		__offset--;
	    } else {
		if (__curs_pos>0)
		    __curs_pos--;
	    }

	    if (realization()==REALIZED)
		refresh(true);
	}

	// From WidgetBase

	void size_available(const Size& _s) {
	    WidgetBase::size_available(__size=_s);
	}

	/**
	 * Size the Input Widget requires.
	 *
	 * @return Either Size::zero() if __length is 0, or the
	 * Size(1, __length).
	 */
	Size size() const {
	    return __size;
	}

	Size size_hint() const {
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
	bool size_change() {
	    return false;
	}

	/**
	 * Reset size.
	 *
	 * If __length is zero, resets __size. Else does nothing.
	 */
	void reset_size() {
	    __size=Size::zero();
	}

	// From Realizeable
	/**
	 * Refresh the Input.
	 *
	 * @param immediate not directly used by Label::refresh() but
	 * passed to Widget::refresh().
	 */
	void refresh(bool immediate) {
	    if (realization()!=REALIZED) return;

	    assert(widget_subwin()!=0);

	    if (werase(widget_subwin())==ERR)
		throw CursesException("werase");

	    std::list<std::string>::iterator it=__list.begin();

	    // Make sure cursor position is not off the list, i.e. on
	    // the border of the widget.
	    __curs_pos=__curs_pos>__cast_lt(__size.rows())-3 ?
		__size.rows()-3 :
		__curs_pos;

	    // Make sure the offset will not produce an out of bound,
	    // for instance due to a screen resize.
	    if (__cast_lt(__size.rows())-2>__list.size()||
		__cast_lt(__size.rows()-2+__offset)>__list.size())
		__offset=0; // we must not use an offset.

	    // Advance to offset
	    for(std::list<std::string>::size_type i=0;
		i<__offset;
		it++, i++);

	    for(std::list<std::string>::size_type i=0;
		i<std::min<std::list<std::string>::size_type>(__size.rows()-2, __list.size());
		it++, i++) {

		if (i==__curs_pos)
		    YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::LISTBOX_HILITE);
		else
		    YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::LISTBOX);

		// We prepare our own line, so that we can fill up
		// with spaces
		std::string line(*it);

		// Find out if we have to indicate that the line has
		// been truncated
		if (line.length()<=__cast_st(__size.cols())-2) {
		    // line is shorter than visible area
		    if (line.length()<__cast_st(__size.cols())-2)
			line.append((__size.cols()-2)-line.length(),' ');
		    mymvwaddnstr(widget_subwin(),
				 i+1,1,
				 line.c_str(), __size.cols()-2);
		} else {
		    mymvwaddnstr(widget_subwin(),
				 i+1,1,
				 line.c_str(), __size.cols()-3);
		    winsch(widget_subwin(), '>');
		}
	    }

	    // This is required in order to color the listbox properly
	    // when highlighting hits the bottom under X/Open Curses
	    YAPET::UI::Colors::set_color(widget_subwin(), YAPET::UI::LISTBOX);

	    //
	    // Box creation and scroll marker setting was moved here,
	    // because it didn't work properly, i.e. there were
	    // artifcats on the right side of the box, when it is
	    // placed in front of the above code block.
	    //
	    if (focus()) {
		if (box(widget_subwin(), 0, 0)==ERR)
		    throw CursesException("box");
	    } else {
		if (box(widget_subwin(), '|', '-')==ERR)
		    throw CursesException("box");
	    }

	    // set scroll markers
	    if (__list.size()>__cast_lt(__size.rows())-2) {
		// Can we scroll up? This is indicated by an __offset
		// > 0
		if (__offset>0)
		    (void)mvwaddch(widget_subwin(), 1, __size.cols()-1, '^');
		// can we scroll further down?
		if (__offset+__size.rows()-3<__list.size()-1)
		    (void)mvwaddch(widget_subwin(), __size.rows()-2, __size.cols()-1, 'v');
	    }

	    // set sort order indicator
	    switch (__sort_order) {
	    case ASCENDING:
		if (mvwaddch(widget_subwin(), 0, 1, '^')==ERR)
		    throw CursesException("mvwaddch");
		break;
	    case DESCENDING:
		if (mvwaddch(widget_subwin(), 0, 1, 'v')==ERR)
		    throw CursesException("mvwaddch");
		break;
	    case UNSORTED:
		break;
	    }

	    // Set the cursor at the right position if we have focus.
	    if (focus() && !__list.empty()) {
		curs_set(1);
		if (leaveok(widget_subwin(), FALSE)==ERR)
		    throw CursesException("leaveok");

		if (wmove(widget_subwin(), __curs_pos+1, 1)==ERR)
		    throw CursesException("wmove");
	    } else {
		curs_set(0);
		if (leaveok(widget_subwin(), TRUE)==ERR)
		    throw CursesException("leaveok");
	    }

	    Widget::refresh(immediate);

	}
};

#undef __cast_lt
#undef __cast_st

#endif // LISTBOX_H
