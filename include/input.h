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

#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <string>

#include "yacurscurses.h"  // required for YACURS_USE_WCHAR
#ifdef YACURS_USE_WCHAR
#include <cwchar>
#include <cwctype>
#else
#include <cctype>
#endif

#include "colors.h"
#include "cursorbuf.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "input.h"
#include "widget.h"
#include "yacursex.h"

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
    virtual bool use(wint_t wc) { return iswprint(wc) != 0; }
#else
    virtual bool use(char c) { return std::isprint(c) != 0; }
#endif
    virtual InputFilter* clone() const { return new FilterPrint; }
};

/**
 * Filter alphabetic characters.
 */
class FilterAlpha : public InputFilter {
   public:
#ifdef YACURS_USE_WCHAR
    virtual bool use(wint_t wc) { return iswalpha(wc) != 0; }
#else
    virtual bool use(char c) { return std::isalpha(c) != 0; }
#endif
    virtual InputFilter* clone() const { return new FilterAlpha; }
};

/**
 * Filter alphanumeric characters.
 */
class FilterAlnum : public InputFilter {
   public:
#ifdef YACURS_USE_WCHAR
    virtual bool use(wint_t wc) { return iswalnum(wc) != 0; }
#else
    virtual bool use(char c) { return std::isalnum(c) != 0; }
#endif
    virtual InputFilter* clone() const { return new FilterAlnum; }
};

/**
 * Filter digit characters.
 */
class FilterDigit : public InputFilter {
   public:
#ifdef YACURS_USE_WCHAR
    virtual bool use(wint_t wc) { return iswdigit(wc) != 0; }
#else
    virtual bool use(char c) { return std::isdigit(c) != 0; }
#endif
    virtual InputFilter* clone() const { return new FilterDigit; }
};

/**
 * Filter hex digit characters.
 */
class FilterXDigit : public InputFilter {
   public:
#ifdef YACURS_USE_WCHAR
    virtual bool use(wint_t wc) { return iswxdigit(wc) != 0; }
#else
    virtual bool use(char c) { return std::isxdigit(c) != 0; }
#endif
    virtual InputFilter* clone() const { return new FilterXDigit; }
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
template <class T = std::string>
class Input : public Widget {
   public:
    typedef typename T::size_type tsz_t;

   private:
    INTERNAL::CursorBuffer _buffer;

    /**
     * Length of the input line.
     *
     * May be zero when dynamically sized.
     */
    int _length;

    /**
     * Mode, whether or not editing is allowed.
     */
    bool _read_only;

    /**
     * Mode, whether or not input is obscurred.
     */
    bool _obscure_input;

    /**
     * Mode, whether or not input is hidden.
     */
    bool _hide_input;

    /**
     * Character used for obscurring input.
     */
    char _obscure_char;

    /**
     * The size of the Input Widget.
     *
     * May be Size::zero() when dynamically sized, else the
     */
    Size _size;

    /**
     * Input filter.
     */
    InputFilter* _filter;

   protected:
    virtual void key_handler(Event& e);

    // From Realizeable
    void realize();

    void unrealize();

   public:
    /**
     * @param length the length (columns) the widget
     * requires. If zero, it will dynamically size. Default 0.
     *
     * @param max_size maximum size of input. Default 255.
     *
     * @param t text to display initially. Default empty.
     */
    Input(int length = 0, tsz_t max_size = 255, const T& t = T());
    Input<T>& operator=(const Input<T>&) = delete;
    Input<T>& operator=(Input<T>&&) = delete;
    Input<T>(const Input<T>&) = delete;
    Input<T>(Input<T>&&) = delete;

    virtual ~Input();

    /**
     * Set the content of the input buffer.
     *
     * @param i the content of the input buffer.
     */
    virtual void input(const T& i);

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
     * @param s @c true, no editing allowed. @c false,
     * editing allowed.
     */
    void readonly(bool s);

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
     * @param m @c true, will obscure input, @c false will show input.
     */
    void obscure_input(bool m);

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
    void obscure_char(char c);

    /**
     * @return character used to obscure input.
     */
    char obscure_char() const;

    void hide_input(bool m);

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
     * @param f reference to filter.
     */
    void filter(const InputFilter& f);

    // From WidgetBase

    void size_available(const Size& s);

    /**
     * Size the Input Widget requires.
     *
     * @return Either Size::zero() if _length is 0, or the
     * Size(1, _length).
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
     * If _length is zero, resets _size. Else does nothing.
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


//
// Protected
//
template <class T>
void Input<T>::key_handler(Event& e) {
    assert(e.type() == EVT_KEY);

    if (!focus()) return;

#ifdef YACURS_USE_WCHAR
    EventEx<wint_t>& ekey = dynamic_cast<EventEx<wint_t>&>(e);
#else
    EventEx<int>& ekey = dynamic_cast<EventEx<int>&>(e);
#endif

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
            if (_read_only) return;
            _buffer.clear();
            break;

        case KEY_CTRL_U:
            if (_read_only) return;
            _buffer.clear_sol();
            break;

        case KEY_EOL:
        case KEY_CTRL_K:
            if (_read_only) return;
            _buffer.clear_eol();
            break;

        case KEY_BKSPC_SOL:
        case KEY_BACKSPACE:
            if (_read_only) return;
            _buffer.backspace();
            break;

        case KEY_CTRL_D:
        case KEY_DC:  // Delete
            if (_read_only) return;
            _buffer.del();
            break;

        case KEY_HOME:
        case KEY_CTRL_A:
            _buffer.home();
            break;

        case KEY_END:
        case KEY_CTRL_E:
            _buffer.end();
            break;

        case KEY_LEFT:
        case KEY_CTRL_B:
            _buffer.back_step();
            break;

        case KEY_RIGHT:
        case KEY_CTRL_F:
            _buffer.forward_step();
            break;

        default:  // regular key presses
            if (_read_only) return;

            // Call the filter
            if (!_filter->use(ekey.data())) return;

            _buffer.insert(ekey.data());
            break;
    }

    refresh(true);
}

template <class T>
void Input<T>::realize() {
    REALIZE_ENTER;

    Widget::realize();

    EventQueue::connect_event(
        EventConnectorMethod1<Input>(EVT_KEY, this, &Input::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    REALIZE_LEAVE;
}

template <class T>
void Input<T>::unrealize() {
    UNREALIZE_ENTER;
    EventQueue::disconnect_event(
        EventConnectorMethod1<Input>(EVT_KEY, this, &Input::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);
    Widget::unrealize();
    UNREALIZE_LEAVE;
}

//
// Public
//
template <class T>
Input<T>::Input(int length, tsz_t max_size, const T& t)
    : Widget(),
      _buffer(t, max_size),
      _length(length),
      _read_only(false),
      _obscure_input(false),
      _hide_input(false),
      _obscure_char('*'),
      _size(_length > 0 ? Size(1, _length) : Size::zero()),
      _filter(new FilterPrint) {
    can_focus(true);
}

template <class T>
Input<T>::~Input() {
    EventQueue::disconnect_event(
        EventConnectorMethod1<Input>(EVT_KEY, this, &Input<T>::key_handler));

    assert(_filter != 0);
    delete _filter;
}

template <class T>
void Input<T>::input(const T& i) {
    _buffer.set(i);

    refresh(true);
}

template <class T>
const T& Input<T>::input() const {
    return _buffer.string();
}

template <class T>
void Input<T>::clear() {
    _buffer.clear();
}

template <class T>
void Input<T>::readonly(bool s) {
    _read_only = s;
}

template <class T>
bool Input<T>::readonly() const {
    return _read_only;
}

template <class T>
void Input<T>::max_input(typename Input<T>::tsz_t max_input) {
    _buffer.max_size(max_input);

    if (realization() == REALIZED) refresh(true);
}

template <class T>
typename Input<T>::tsz_t Input<T>::max_input() const {
    return _buffer.max_size();
}

template <class T>
void Input<T>::obscure_input(bool m) {
    if (m && _hide_input) _hide_input = false;

    _obscure_input = m;

    if (realization() == REALIZED) refresh(true);
}

template <class T>
bool Input<T>::obscure_input() const {
    return _obscure_input;
}

template <class T>
void Input<T>::obscure_char(char c) {
    _obscure_char = c;
}

template <class T>
char Input<T>::obscure_char() const {
    return _obscure_char;
}

template <class T>
void Input<T>::hide_input(bool m) {
    if (m && _obscure_input) _obscure_input = false;

    _hide_input = m;

    if (realization() == REALIZED) refresh(true);
}

template <class T>
bool Input<T>::hide_input() const {
    return _hide_input;
}

template <class T>
bool Input<T>::changed() const {
    return _buffer.changed();
}

template <class T>
void Input<T>::filter(const InputFilter& f) {
    assert(_filter != 0);
    delete _filter;

    _filter = f.clone();
    assert(_filter != 0);
}

template <class T>
void Input<T>::size_available(const Size& s) {
    WidgetBase::size_available(s);

    // if we're dynamic, take the cols from s as cols for our
    // _size. Else, do nothing.
    if (_length == 0) {
        _size.rows(1);  // this is fixed for Input Widgets
        _size.cols(s.cols());
    }
    assert(_size.rows() <= s.rows());
    assert(_size.cols() <= s.cols());
}

template <class T>
Size Input<T>::size() const {
    return _size;
}

template <class T>
Size Input<T>::size_hint() const {
    // if _length != 0 then we're not dynmically sized, and the ctor
    // has set the size, which we return. See also comment for
    // size_hint() in widgetbase.h.
    if (_length != 0) return _size;

    return Size(1, _buffer.length());
}

template <class T>
bool Input<T>::size_change() {
    return false;
}

template <class T>
void Input<T>::reset_size() {
    // If _length != 0 then we are not dynamically sized, thus not
    // resetting size.
    if (_length != 0) return;

    _size = Size::zero();
}

template <class T>
void Input<T>::refresh(bool immediate) {
    if (realization() != REALIZED) return;
    assert(widget_subwin() != 0);

    // setting background is essential for coloring entire input
    // widget.

    if (focus()) {
        curs_set(1);
        widget_subwin()->leaveok(false);
    } else {
        curs_set(0);
        widget_subwin()->leaveok(true);
    }

    int16_t curs_pos;

    // Make sure that in the following if/else block curs_pos is
    // set in any case.

    if (_obscure_input) {
        int16_t len;

        _buffer.info(_size.cols(), &len, &curs_pos);
        CurStr obs_out(
            "", Coordinates(),
            focus() ? YACURS::INPUTWIDGET_FOCUS : YACURS::INPUTWIDGET_NOFOCUS);
        obs_out.assign(len, _obscure_char);
        widget_subwin()->addlinex(obs_out);
    } else {
        CurStr out(
            _buffer.string(_size.cols(), &curs_pos), Coordinates(),
            focus() ? YACURS::INPUTWIDGET_FOCUS : YACURS::INPUTWIDGET_NOFOCUS);
        if (_hide_input) {
            // first, give the widget nice color
            CurStr filler("", Coordinates(),
                          focus() ? YACURS::INPUTWIDGET_FOCUS
                                  : YACURS::INPUTWIDGET_NOFOCUS);
            widget_subwin()->addlinex(filler);
            // Now, put the string with hidden attributes
            out.color(YACURS::INPUTWIDGET_HIDDEN);
            widget_subwin()->addstr(out);
        } else {
            widget_subwin()->addlinex(out);
        }
    }

    widget_subwin()->move(Coordinates(curs_pos, 0));

    Widget::refresh(immediate);
}
}  // namespace YACURS

#endif  // INPUT_H
