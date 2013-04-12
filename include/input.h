// -*- mode: c++ -*-
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

#ifndef INPUT_H
#define INPUT_H

#include <string>
#include <cassert>
#include <cstdlib>

#include "input.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursex.h"
#include "colors.h"
#include "widget.h"

namespace YACURS {
    /**
     * Input line.
     *
     * Widget providing one line for text input.
     *
     * If not length is specified, it behaves like a dynamic
     * Widget. Providing a length make it behave non-dynamic.
     *
     * Following cursor motion keys are supported:
     *
     *  - Key left: move cursor to the left
     *  - Key right: move cursor to the right
     *  - Ctrl-A: move to the beginning of line
     *  - Ctrl-E: move to the end of line
     *
     * Following editing keys are supported:
     *
     *  - Ctrl-U: delete all text
     *  - Ctrl-K: delete text from current cursor position to end of text.
     *  - Backspace: delete character left to the cursor
     *  - Delete: delete character right to the cursor
     *
     * Misc keys:
     *
     *  - Key Down, Enter, Tab: focus next Widget.
     *  - Key Up, Shift-Tab: focus previous Widget.
     *
     * When Input is put in read-only mode, all editing keys keys are
     * disabled. Only cursor motion is available.
     */
    template<class T=std::string>
    class Input: public Widget {
	public:
	    typedef typename T::size_type tsz_t;
	private:
	    /**
	     * Offset into the buffer. Used when not the entire buffer can
	     * be displayed in the Input Widget, i.e. scrolling.
	     */
	    tsz_t __offset;

	    /**
	     * Cursor position relative to offset, absolute to Widget.
	     */
	    tsz_t __curs_pos;

	    /**
	     * Maximum size of input.
	     *
	     * If it is 0, there is no limit.
	     */
	    tsz_t __max_size;

	    /**
	     * Length of the input line.
	     *
	     * May be zero when dynamically sized.
	     */
	    int __length;

	    /**
	     * Mode, whether or not editing is allowed.
	     */
	    bool __read_only;

	    /**
	     * Mode, whether or not input is obscurred.
	     */
	    bool __obscure_input;

	    /**
	     * Mode, whether or not input is hidden.
	     */
	    bool __hide_input;

	    /**
	     * Character used for obscurring input.
	     */
	    char __obscure_char;

	    /**
	     * The input
	     */
	    T __buffer;

	    /**
	     * The size of the Input Widget.
	     *
	     * May be Size::zero() when dynamically sized, else the
	     */
	    Size __size;

	    // Not supported
	    Input<T>& operator=(const Input<T>& _i);
	protected:
	    virtual void key_handler(Event& _e);

	    // From Realizeable
	    void realize();
	    void unrealize();

	public:
	    /**
	     * @param _length the length (columns) the widget requires. If
	     * zero, it will dynamically size. Default 0.
	     *
	     * @param _max_size maximum size of input. Default 255.
	     *
	     * @param _t text to display initially. Default empty.
	     */
	    Input(int _length=0, tsz_t _max_size=255,
		  const T& _t=std::string());
	    virtual ~Input();

	    /**
	     * Set the content of the input buffer.
	     *
	     * @param _i the content of the input buffer.
	     */
	    virtual void input(const T& _i);

	    /**
	     * Return the content of the input buffer.
	     *
	     * @return the content of the input buffer.
	     */
	    virtual const T& input() const;

	    /**
	     * Clear the input buffer.
	     */
	    virtual void clear();

	    /**
	     * Set readonly mode.
	     *
	     * @param _s @c true, no editing allowed. @c false, editing
	     * allowed.
	     */
	    void readonly(bool _s);

	    /**
	     * Get read-only mode
	     *
	     * @return @c true if Input Widget is in read-only mode, @c
	     * false otherwise.
	     */
	    bool readonly() const;

	    /**
	     * Set input obscure mode
	     *
	     * @param _m @c true, will obscure input, @c false will show input.
	     */
	    void obscure_input(bool _m);

	    /**
	     * Get obscure input mode.
	     *
	     * @return @c true if obscure input mode is enabled, @c false
	     * otherwise.
	     */
	    bool obscure_input() const;

	    /**
	     * @param c character used to obscure input. Only used when
	     * obscuring input
	     */
	    void obscure_char(char _c);

	    /**
	     * @return character used to obscure input.
	     */
	    char obscure_char() const;

	    void hide_input(bool _m);
	    bool hide_input() const;

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
	    /**
	     * Refresh the Input.
	     *
	     * @param immediate not directly used by Label::refresh() but
	     * passed to Widget::refresh().
	     */
	    void refresh(bool immediate);
    };

    //
    // Private
    //

    template<class T> Input<T>&
    Input<T>::operator=(const Input<T>&) {
	throw NotSupported();
	return *this;
    }

    //
    // Protected
    //
    template<class T> void
    Input<T>::key_handler(Event& _e) {
	assert(_e.type()==EVT_KEY);

	if (!focus()) return;

	EventEx<int>& ekey=dynamic_cast<EventEx<int>&>(_e);

	assert(__offset+__curs_pos<=__buffer.length());

	switch (ekey.data()) {
	case KEY_DOWN:
	case KEY_ENTER:
	case KEY_RETURN:
	case KEY_RETURN2:
	case KEY_TAB:
	    EventQueue::submit(EVT_FOCUS_NEXT);
	    break;
	case KEY_UP:
	case KEY_BTAB:
	    EventQueue::submit(EVT_FOCUS_PREVIOUS);
	    break;
	case KEY_DL:
	case KEY_CTRL_U:
	    if (__read_only) break;
	    __buffer.clear();
	    __curs_pos=0;
	    __offset=0;
	    break;
	case KEY_EOL:
	case KEY_CTRL_K:
	    if (__read_only) break;
	    __buffer=__buffer.erase(__offset+__curs_pos,
				    __buffer.length()-(__offset+__curs_pos));
	    break;
	case KEY_BKSPC_SOL:
	case KEY_BACKSPACE:
	    if (__read_only) break;
	    if (__offset==0 && __curs_pos==0) break;

	    if (__offset>0) {
		__offset--;
	    } else {
		if (__curs_pos>0)
		    __curs_pos--;
	    }
	    if (!__buffer.empty())
		__buffer=__buffer.erase(__offset+__curs_pos,1);
	    break;
	case KEY_CTRL_D:
	case KEY_DC: // Delete
	    if (__read_only) break;
	    if (__offset+__curs_pos>=__buffer.length() ||
		__buffer.empty()) break;
	    __buffer=__buffer.erase(__offset+__curs_pos,1);
	    break;
	case KEY_HOME:
	case KEY_CTRL_A:
	    __curs_pos=0;
	    __offset=0;
	    break;
	case KEY_CTRL_E:
	    if (__buffer.length()>=static_cast<tsz_t>(__size.cols())) {
		__offset=__buffer.length()-__size.cols()+1;
		__curs_pos=__size.cols();
	    } else {
		__offset=0;
		__curs_pos=__buffer.length();
	    }
	    break;
	case KEY_LEFT:
	case KEY_CTRL_B:
	    if (__curs_pos>0) {
		__curs_pos--;
	    } else {
		if (__offset>0) {
		    __offset--;
		} else {
		    if (__curs_pos>0)
			__curs_pos--;
		}
	    }
	    break;
	case KEY_RIGHT:
	case KEY_CTRL_F:
	    if (__curs_pos+__offset>=__buffer.length()) break;

	    if (__curs_pos+1==static_cast<tsz_t>(__size.cols())) {
		__offset++;
	    } else {
		// we're somewhere in the widget, but not the end, advance
		// the cursor position
		__curs_pos++;
	    }
	    break;
	default: // regular key presses
	    if (__read_only) break;

	    // do not overrun the max size
	    if (__buffer.length()>=__max_size) break;

	    // Add the char to the curses window
	    //
	    // (void) used to silence clang w/ -Wall -pedantic
	    widget_subwin()->mvaddch(Coordinates(__curs_pos,0), ekey.data());

	    // Add (insert) the char to the buffer. No cursor motion, this
	    // is done in the next block.
	    if (__offset+__curs_pos>__buffer.length()) {
		// we would exceed the buffer, so we push it back at the end
		assert(__offset+__curs_pos==__buffer.length()+1);
		__buffer.push_back(ekey.data());
	    } else {
		// we're somewhere in the middle of the buffer, insert the
		// char there.
		__buffer.insert(__offset+__curs_pos, 1, ekey.data());
	    }

	    // Make sure the __curs_pos does not overshoot the
	    // border of the window
	    assert(__curs_pos<static_cast<tsz_t>(__size.cols()));

	    // Advance the cursor position. If __curs_pos+1 hits
	    // the border, advance the offset. This way we always have a
	    // space at the end of the string (on the screen only, not in
	    // the __buffer of course).
	    if (__curs_pos+1==static_cast<tsz_t>(__size.cols())) {
		__offset++;
	    } else {
		// we're somewhere in the widget, but not the end, advance
		// the cursor position
		__curs_pos++;
	    }
	    break;
	}

	refresh(true);
    }

    template<class T> void
    Input<T>::realize() {
	REALIZE_ENTER;
	
	Widget::realize();

	EventQueue::connect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));

	assert(focusgroup_id()!=(fgid_t)-1);

	REALIZE_LEAVE;
    }

    template<class T> void
    Input<T>::unrealize() {
	UNREALIZE_ENTER;
	EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input::key_handler));

	assert(focusgroup_id()!=(fgid_t)-1);

	Widget::unrealize();
	UNREALIZE_LEAVE;
    }

    //
    // Public
    //
    template<class T>
    Input<T>::Input(int _length, tsz_t _max_size, const T& _t):
	Widget(),
	__offset(0),
	__curs_pos(0),
	__max_size(_max_size),
	__length(_length),
	__read_only(false),
	__obscure_input(false),
	__hide_input(false),
	__obscure_char('*'),
	__buffer(_t.length()>__max_size?_t.substr(0,__max_size):_t),
	__size(__length>0 ? Size(1, __length) : Size::zero()) {
	can_focus(true);
    }

    template<class T>
    Input<T>::~Input() {
	EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,this, &Input<T>::key_handler));
    }

    template<class T> void
    Input<T>::input(const T& i) {
	if (i.length()>__max_size)
	    __buffer=i.substr(0,__max_size);
	else
	    __buffer=i;

	// Reset cursor position
	__offset=0;
	__curs_pos=0;

	refresh(true);
    }

    template<class T> const T&
    Input<T>::input() const {
	return __buffer;
    }

    template<class T> void
    Input<T>::clear() {
	__buffer.clear();
	__curs_pos=0;
	__offset=0;
    }

    template<class T> void
    Input<T>::readonly(bool _s) {
	__read_only=_s;
    }

    template<class T> bool
    Input<T>::readonly() const {
	return __read_only;
    }

    template<class T> void
    Input<T>::obscure_input(bool _m) {
	if (_m && __hide_input)
	    __hide_input=false;
	__obscure_input=_m;
    }

    template<class T> bool
    Input<T>::obscure_input() const {
	return __obscure_input;
    }

    template<class T> void
    Input<T>::obscure_char(char _c) {
	__obscure_char=_c;
    }

    template<class T> char
    Input<T>::obscure_char() const {
	return __obscure_char;
    }

    template<class T> void
    Input<T>::hide_input(bool _m) {
	if (_m && __obscure_input)
	    __obscure_input=false;
	__hide_input=_m;
    }

    template<class T> bool
    Input<T>::hide_input() const {
	return __hide_input;
    }

    template<class T> void
    Input<T>::size_available(const Size& _s) {
	WidgetBase::size_available(_s);

	// if we're dynamic, take the cols from _s as cols for our
	// __size. Else, do nothing.
	if (__length==0) {
	    __size.rows(1); // this is fixed for Input Widgets
	    __size.cols(_s.cols());
	} 
	assert(__size.rows()<=_s.rows());
	assert(__size.cols()<=_s.cols());
    }

    template<class T> Size
    Input<T>::size() const {
	return __size;
    }

    template<class T> Size
    Input<T>::size_hint() const {
	// if __length != 0 then we're not dynmically sized, and the ctor
	// has set the size, which we return. See also comment for
	// size_hint() in widgetbase.h.
	if (__length!=0) return __size;

	return Size(1,__buffer.length());
    }

    template<class T> bool
    Input<T>::size_change() {
	return false;
    }

    template<class T> void
    Input<T>::reset_size() {
	// If __length != 0 then we are not dynamically sized, thus not
	// resetting size.
	if (__length!=0) return;

	__size=Size::zero();
    }

    template<class T> void
    Input<T>::refresh(bool immediate) {
	if (realization()!=REALIZED) return;
	assert(widget_subwin()!=0);

	// setting background is essential for coloring entire input
	// widget.

	if (focus()) {
	    curs_set(1);
	    widget_subwin()->leaveok(false);
	} else {
	    curs_set(0);
	    widget_subwin()->leaveok(true);
	}

	widget_subwin()->erase();

	assert(__offset<=__buffer.length());
	// if (mymvwaddstr(widget_subwin(), 0, 0,
	// 		    __label.c_str()) == ERR)
	// 	throw AddStrFailed();
	//
	// We ignore the error returned, since the cursor cannot be
	// advanced past the end, and thus the string is
	// truncated. However, the truncation has no effect on the input.
	if (__buffer.length()>0) {
	    std::string* output=&__buffer;
	    std::string obscure_out; //only used when obscuring output

	    if (__obscure_input) {
		tsz_t outlen=
		    __buffer.substr(__offset, __size.cols()-1).length();
		obscure_out.assign(outlen, __obscure_char);
		output=&obscure_out;
	    }

	    YACURS::INTERNAL::CurStr
		out(output->substr(__offset,
				   __size.cols()-1).c_str(),
		    Coordinates(),
		    focus()?YACURS::INPUTWIDGET_FOCUS:YACURS::INPUTWIDGET_NOFOCUS);
	    
	    if (__hide_input) {
		// first, give the widget nice color
		YACURS::INTERNAL::CurStr
		    filler("",
			   Coordinates(),
			   focus()?YACURS::INPUTWIDGET_FOCUS:YACURS::INPUTWIDGET_NOFOCUS);
		widget_subwin()->addlinex(filler);
		// Now, put the string with hidden attributes
		out.color(YACURS::INPUTWIDGET_HIDDEN);
		widget_subwin()->addstr(out);
	    } else {
		widget_subwin()->addlinex(out);
	    }

	    // Sanitize the cursor position if necessary, for example due
	    // to a shrink of the screen, the cursor position might
	    // overshoot the available subwin size.
	    if (__curs_pos>=static_cast<tsz_t>(__size.cols()) ) __curs_pos=__size.cols()-1;
	} else {
	    // Fill the widget with spaces to give it nice color
	    YACURS::INTERNAL::CurStr out("",Coordinates(),focus()?YACURS::INPUTWIDGET_FOCUS:YACURS::INPUTWIDGET_NOFOCUS);
	    widget_subwin()->addlinex(out);
	    // since the buffer is empty, make sure the cursor position is
	    // set to the biginning.
	    assert(__curs_pos==0); 
	}    
	widget_subwin()->move(Coordinates(__curs_pos,0));

	Widget::refresh(immediate);
    }
}

#endif // INPUT_H
