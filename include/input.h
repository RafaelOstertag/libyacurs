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

#ifndef INPUT_H
#define INPUT_H 1

#include <string>
#include <cassert>
#include <cstdlib>
#include <cerrno>
#ifdef YACURS_USE_WCHAR
# include <cwchar>
# include <cwctype>
#else
# include <cctype>
#endif

#include "input.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursex.h"
#include "colors.h"
#include "widget.h"
#include "cursorbuf.h"

namespace YACURS {
    /**
     * Base class for filtering input.
     */
    class InputFilter {
	public:
	    /**
	     * Filter method.
	     *
	     * Determines whether (@c true) or not (@c false) the
	     * input is accepted.
	     *
	     * @param c character
	     *
	     * @retrun @c true if the input has to be accepted, or @c
	     * false if it is to be discarded.
	     */
#ifdef YACURS_USE_WCHAR
	    virtual bool use(wint_t) = 0;
#else
	    virtual bool use(char) = 0;
#endif
	    virtual InputFilter* clone() const = 0;
	    virtual ~InputFilter() {}
    };

    /**
     * Filter printable characters.
     */
    class FilterPrint : public InputFilter {
	public:
#ifdef YACURS_USE_WCHAR
	    virtual bool use(wint_t wc) {
		return iswprint(wc) != 0;
	    }
#else
	    virtual bool use(char c) {
		return isprint(c) != 0;
	    }
#endif
	    virtual InputFilter* clone() const {
		return new FilterPrint;
	    }
    };

    /**
     * Filter alphabetic characters.
     */
    class FilterAlpha : public InputFilter {
	public:
#ifdef YACURS_USE_WCHAR
	    virtual bool use(wint_t wc) {
		return iswalpha(wc) != 0;
	    }
#else
	    virtual bool use(char c) {
		return isalpha(c) != 0;
	    }
#endif
	    virtual InputFilter* clone() const {
		return new FilterAlpha;
	    }
    };

    /**
     * Filter alphanumeric characters.
     */
    class FilterAlnum : public InputFilter {
	public:
#ifdef YACURS_USE_WCHAR
	    virtual bool use(wint_t wc) {
		return iswalnum(wc) != 0;
	    }
#else
	    virtual bool use(char c) {
		return isalnum(c) != 0;
	    }
#endif
	    virtual InputFilter* clone() const {
		return new FilterAlnum;
	    }
    };

    /**
     * Filter digit characters.
     */
    class FilterDigit : public InputFilter {
	public:
#ifdef YACURS_USE_WCHAR
	    virtual bool use(wint_t wc) {
		return iswdigit(wc) != 0;
	    }
#else
	    virtual bool use(char c) {
		return isdigit(c) != 0;
	    }
#endif
	    virtual InputFilter* clone() const {
		return new FilterDigit;
	    }
    };

    /**
     * Filter hex digit characters.
     */
    class FilterXDigit : public InputFilter {
	public:
#ifdef YACURS_USE_WCHAR
	    virtual bool use(wint_t wc) {
		return iswxdigit(wc) != 0;
	    }
#else
	    virtual bool use(char c) {
		return isxdigit(c) != 0;
	    }
#endif
	    virtual InputFilter* clone() const {
		return new FilterXDigit;
	    }
    };
	    
	    
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
     * When Input is put in read-only mode, all editing keys are
     * disabled. Only cursor motion is available.
     *
     * Custom input filter can be used to filter user input. However,
     * filter do not take effect on data set using the input()
     * method. Also, filters do not affect any data already in the
     * buffer when a new filter is set.
     */
    template<class T = std::string> class Input : public Widget {
        public:
            typedef typename T::size_type tsz_t;
        private:
            INTERNAL::CursorBuffer __buffer;

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
             * The size of the Input Widget.
             *
             * May be Size::zero() when dynamically sized, else the
             */
            Size __size;

	    /**
	     * Input filter.
	     */
	    InputFilter* __filter;

            // Not supported
            Input<T>& operator=(const Input<T>& _i);

        protected:
            virtual void key_handler(Event& _e);

            // From Realizeable
            void realize();

            void unrealize();

        public:
            /**
             * @param _length the length (columns) the widget
             * requires. If zero, it will dynamically size. Default 0.
             *
             * @param _max_size maximum size of input. Default 255.
             *
             * @param _t text to display initially. Default empty.
             */
            Input(int _length=0, tsz_t _max_size=255,
                  const T& _t=T() );
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
             * @param _s @c true, no editing allowed. @c false,
             * editing allowed.
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
	     * Set maximum size of input.
	     *
	     * Set maximum size (characters) of input.
	     *
	     * @param max_input new maximum size of input
	     */
	    void max_input(tsz_t max_input);

	    /**
	     * Get maximum input size.
	     */
	    tsz_t max_input() const;

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

	    /**
	     * Indicate whether or not input has changed.
	     */
	    bool changed() const;

	    /**
	     * Set a new character filter.
	     *
	     * Setting a new filter does not affect characters already
	     * in buffer.
	     *
	     * @param _f reference to filter.
	     */
	    void filter(const InputFilter& _f);


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
        throw EXCEPTIONS::NotSupported();
        return *this;
    }

    //
    // Protected
    //
    template<class T> void
    Input<T>::key_handler(Event& _e) {
        assert(_e.type() == EVT_KEY);

        if (!focus() ) return;

#ifdef YACURS_USE_WCHAR
        EventEx<wint_t>& ekey = dynamic_cast<EventEx<wint_t>&>(_e);
#else
        EventEx<int>& ekey = dynamic_cast<EventEx<int>&>(_e);
#endif

        switch (ekey.data() ) {
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
            if (__read_only) return;
            __buffer.clear();
            break;

        case KEY_CTRL_U:
            if (__read_only) return;
            __buffer.clear_sol();
            break;

        case KEY_EOL:
        case KEY_CTRL_K:
            if (__read_only) return;
            __buffer.clear_eol();
            break;

        case KEY_BKSPC_SOL:
        case KEY_BACKSPACE:
            if (__read_only) return;
            __buffer.backspace();
            break;

        case KEY_CTRL_D:
        case KEY_DC: // Delete
            if (__read_only) return;
            __buffer.del();
            break;

        case KEY_HOME:
        case KEY_CTRL_A:
            __buffer.home();
            break;

        case KEY_CTRL_E:
            __buffer.end();
            break;

        case KEY_LEFT:
        case KEY_CTRL_B:
            __buffer.back_step();
            break;

        case KEY_RIGHT:
        case KEY_CTRL_F:
            __buffer.forward_step();
            break;

        default: // regular key presses
            if (__read_only) return;

	    // Call the filter
	    if (!__filter->use(ekey.data())) return;

	    __buffer.insert(ekey.data() );
            break;
        }

        refresh(true);
    }

    template<class T> void
    Input<T>::realize() {
        REALIZE_ENTER;

        Widget::realize();

        EventQueue::connect_event(EventConnectorMethod1<Input>(EVT_KEY, this,
                                                               &Input::
                                                               key_handler) );

        assert(focusgroup_id() != FocusManager::nfgid);

        REALIZE_LEAVE;
    }

    template<class T> void
    Input<T>::unrealize() {
        UNREALIZE_ENTER;
        EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,
                                                                  this,
                                                                  &Input::
                                                                  key_handler) );

        assert(focusgroup_id() != FocusManager::nfgid);
	Widget::unrealize();
        UNREALIZE_LEAVE;
    }

    //
    // Public
    //
    template<class T> Input<T>::Input(int _length,
                                      tsz_t _max_size,
                                      const T& _t) :
        Widget(),
        __buffer(_t, _max_size),
        __length(_length),
        __read_only(false),
        __obscure_input(false),
        __hide_input(false),
        __obscure_char('*'),
        __size(__length > 0 ? Size(1, __length) : Size::zero() ),
	__filter(new FilterPrint) {
        can_focus(true);
    }

    template<class T> Input<T>::~Input() {
        EventQueue::disconnect_event(EventConnectorMethod1<Input>(EVT_KEY,
                                                                  this,
                                                                  &Input<T>::
                                                                  key_handler) );

	assert(__filter!=0);
	delete __filter;
    }

    template<class T> void
    Input<T>::input(const T& i) {
        __buffer.set(i);

        refresh(true);
    }

    template<class T> const T&
    Input<T>::input() const {
        return __buffer.string();
    }

    template<class T> void
    Input<T>::clear() {
        __buffer.clear();
    }

    template<class T> void
    Input<T>::readonly(bool _s) {
        __read_only = _s;
    }

    template<class T> bool
    Input<T>::readonly() const {
        return __read_only;
    }

    template<class T> void
    Input<T>::max_input(typename Input<T>::tsz_t max_input) {
	__buffer.max_size(max_input);

	if (realization() == REALIZED)
	    refresh(true);
    }

    template<class T> typename Input<T>::tsz_t
    Input<T>::max_input() const {
	return __buffer.max_size();
    }

    template<class T> void
    Input<T>::obscure_input(bool _m) {
        if (_m && __hide_input)
            __hide_input = false;

        __obscure_input = _m;

	if (realization() == REALIZED)
	    refresh(true);
    }

    template<class T> bool
    Input<T>::obscure_input() const {
        return __obscure_input;
    }

    template<class T> void
    Input<T>::obscure_char(char _c) {
        __obscure_char = _c;
    }

    template<class T> char
    Input<T>::obscure_char() const {
        return __obscure_char;
    }

    template<class T> void
    Input<T>::hide_input(bool _m) {
        if (_m && __obscure_input)
            __obscure_input = false;

        __hide_input = _m;

	if (realization() == REALIZED)
	    refresh(true);
    }

    template<class T> bool
    Input<T>::hide_input() const {
        return __hide_input;
    }

    template<class T> bool
    Input<T>::changed() const {
	return __buffer.changed();
    }

    template<class T> void 
    Input<T>::filter(const InputFilter& _f) {
	assert(__filter!=0);
	delete __filter;

	__filter = _f.clone();
	assert(__filter!=0);
    }

    template<class T> void
    Input<T>::size_available(const Size& _s) {
        WidgetBase::size_available(_s);

        // if we're dynamic, take the cols from _s as cols for our
        // __size. Else, do nothing.
        if (__length == 0) {
            __size.rows(1); // this is fixed for Input Widgets
            __size.cols(_s.cols() );
        }
        assert(__size.rows() <= _s.rows() );
        assert(__size.cols() <= _s.cols() );
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
        if (__length != 0) return __size;

        return Size(1, __buffer.length() );
    }

    template<class T> bool
    Input<T>::size_change() {
        return false;
    }

    template<class T> void
    Input<T>::reset_size() {
        // If __length != 0 then we are not dynamically sized, thus not
        // resetting size.
        if (__length != 0) return;

        __size = Size::zero();
    }

    template<class T> void
    Input<T>::refresh(bool immediate) {
        if (realization() != REALIZED) return;
        assert(widget_subwin() != 0);

        // setting background is essential for coloring entire input
        // widget.

        if (focus() ) {
            curs_set(1);
            widget_subwin()->leaveok(false);
        } else {
            curs_set(0);
            widget_subwin()->leaveok(true);
        }

        int16_t curs_pos;

        // Make sure that in the following if/else block curs_pos is
        // set in any case.

        if (__obscure_input) {
            int16_t len;

            __buffer.info(__size.cols(), &len, &curs_pos);
            CurStr obs_out("", Coordinates(), focus() ?
                           YACURS::INPUTWIDGET_FOCUS :
                           YACURS::INPUTWIDGET_NOFOCUS);
            obs_out.assign(len, __obscure_char);
            widget_subwin()->addlinex(obs_out);
        } else {
            CurStr
                out(__buffer.string(__size.cols(), &curs_pos),
                    Coordinates(),
                    focus() ? YACURS::INPUTWIDGET_FOCUS : YACURS::INPUTWIDGET_NOFOCUS);
            if (__hide_input) {
                // first, give the widget nice color
                CurStr
                    filler("",
                           Coordinates(),
                           focus() ? YACURS::INPUTWIDGET_FOCUS : YACURS::INPUTWIDGET_NOFOCUS);
                widget_subwin()->addlinex(filler);
                // Now, put the string with hidden attributes
                out.color(YACURS::INPUTWIDGET_HIDDEN);
                widget_subwin()->addstr(out);
            } else {
                widget_subwin()->addlinex(out);
            }
        }

        widget_subwin()->move(Coordinates(curs_pos, 0) );

        Widget::refresh(immediate);
    }
}

#endif // INPUT_H
