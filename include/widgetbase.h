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

#ifndef WIDGETBASE_H
#define WIDGETBASE_H 1

#include <string>

#include "area.h"
#include "curswin.h"
#include "focusmanager.h"
#include "realizeable.h"
#include "window.h"
#include "yacurscurses.h"
#include "yacurstypes.h"

namespace YACURS {
// Forward declaration because window.h already included widgetbase.h
class Window;

/**
 * A widget is an object with or without interaction on the screen.
 *
 * WidgetBase provides the interface common to all Widgets.
 *
 * @subsection Curses Window
 *
 * If derived classes need Curses Windows, it is expected that they
 * create the Curses Window using subwin() on the Curses Window
 * returned by curses_window().
 *
 * @subsection Widget Position
 *
 * The Window the Widget is associated, or the Parent Widget, calls
 * position() on the Widget in order to position it.
 *
 * @subsection Widget Size
 *
 * Information on how much space the widget has to its disposition,
 * has to be provided by the Window the widget is associated or by the
 * parent Widget by calling size_available().
 *
 * Dynamically sized Widgets return Size::zero() upon a call to
 * size(). An implementation detail of those Widget is, that they
 * should reset their size() to Size::zero() upon unrealize() or when
 * explicitely requested by reset_size().
 *
 * If a Widget can provide size hints, the hints has to be provided
 * when size_hint() is called. If no hint can be provided for a
 * dynamic Widget, Size::zero() has to be returned.
 *
 * Non-dynamic Widget have to return size_hint() == size().
 *
 * @subsection Focus
 *
 * If a Widget can have focus, can_focus() must return @c true. The
 * Widget must add itself to the Focus Group upon realize() and remove
 * itself from the Focus Group upon unrealize(). The Parent Widget or
 * the Window has to provide the proper Focus Manager ID.
 *
 * For details on Focus Management, see FocusGroup and FocusManager.
 *
 * Widgets that want to handle keyboard input, should be focusable and
 * connect to the EVT_KEY event. However, they must only react to
 * EVT_KEY if they have focus.
 *
 * @subsection Screen Resize
 *
 * Widgets must not connect to @c EVT_SIGWINCH. A resize is initiated
 * by the Window, or the Parent Widget.
 */
class WidgetBase : public Realizeable {
   private:
    /**
     * Pointer used for subwin() in order to create
     * subwindows.
     *
     * Curses window used to display the widget. Widgets like
     * Pack may not directly use this, but pass the pointer on
     * to associated Widgets. WidgetBase does not maintain the
     * window, i.e. it does not free the memory
     * occupied. However, the pointer has to be valid for the
     * entire lifetime of the object.
     */
    YACURS::INTERNAL::CursWin* _curses_window;

    /**
     * Focus Group ID
     *
     * Focus Group where this Widget has to register.
     *
     * @note It should be set to @c FocusManager::nfgid when
     * unrealize()'d.
     */
    FocusManager::fgid_t _fgid;

    /**
     * Flag indicating whether or not the Widget can be focused.
     *
     * Set to @c false by ctor.
     */
    bool _can_focus;

    /**
     * Focus flag.
     *
     * Indicator whether or not the widget has Focus.
     *
     * Set to @c false by ctor.
     */
    bool _focus;

    /**
     * The parent of the widget. If the widget has no parent, it
     * has to be set to 0.
     *
     * Basically, only container widgets such as Packs can be
     * parents.
     */
    WidgetBase* _parent;

    /**
     * Holds the position on the screen, where the widget will
     * display.
     */
    Coordinates _position;

    /**
     * Holds the Size available to the widget.
     *
     * Holds the Size available to the widget starting from
     * _position.
     *
     * This is mostly used for container widgets such as Pack or
     * Widgets dynamically adjust their size.
     *
     * This Size is also imperative for the Widget. If the size()
     * of a Widget is component wise greater than
     * _size_available, the behavior is undefined.
     */
    Size _size_available;

   protected:
    /**
     * Get pointer to the parent of the widget.
     *
     * If the widget has no parent, 0 is returned.
     *
     * @return the address of the parent widget or 0 if the is
     * no parent.
     */
    WidgetBase* parent() const;

    /**
     * Get the curses window of the Widget.
     *
     * This is the pointer to the curses window, where the
     * subwin() will be created.
     *
     * @return pointer to the curses window.
     */
    YACURS::INTERNAL::CursWin* curses_window() const;

    /**
     * Get the Focus Group ID.
     *
     * @return Focus Group ID.
     */
    FocusManager::fgid_t focusgroup_id() const;

    /**
     * Set whether or not Widget can be focused
     */
    void can_focus(bool can_focus);

   public:
    /**
     * Constructor.
     *
     * Upon construction, Widgets have no
     *
     * - focus
     * - curses window
     * - parent
     * - position (i.e. Coordinates::zero())
     * - size available (i.e. Size::zero())
     */
    WidgetBase();
    WidgetBase(const WidgetBase&) = delete;
    WidgetBase& operator=(const WidgetBase&) = delete;
    WidgetBase(WidgetBase&&) = delete;
    WidgetBase& operator=(WidgetBase&&) = delete;


    virtual ~WidgetBase();

    /**
     * Set parent of widget.
     *
     * A parent is usually a container widget such as Packs.
     *
     * This is usually not called by the user.
     *
     * @param p the parent. The pointer has to be valid during
     * the lifetime of the widget.
     */
    void parent(WidgetBase* p);

    /**
     * Set the curses window of the widget.
     *
     * The curses window is used for displaying the widget.
     *
     * This is usually not called by the user and is used for
     * creating subwin().
     *
     * @param p pointer to curses window. The pointer has to be
     * valid for the entire lifetime of the widget.
     *
     * @internal
     *
     * it is virtual because Pack implements its own version
     */
    virtual void curses_window(YACURS::INTERNAL::CursWin* p);

    /**
     * Set Focus Group ID.
     *
     * @param id Focus Group ID.
     *
     * @internal
     *
     * it is virtual because Pack implements its own version.
     */
    virtual void focusgroup_id(FocusManager::fgid_t id);

    /**
     * Set the position, where the widget will be displayed.
     *
     * @param c position where the widget will be displayed. The
     * position is relative to _curses_window, since it is
     * expected that widgets use derwin().
     */
    void position(const Coordinates& c);

    /**
     * Get the the position of the widget.
     *
     * @return the position of the widget. The position is
     * relative to the position of _curses_window.
     */
    const Coordinates& position() const;

    /**
     * Set the size available to the widget.
     *
     * This is used by dynamic widgets and has to be set by the
     * parent widget, or the Window if the widget has no parent
     * Widget.
     *
     * @param s size available to the widget
     *
     */
    virtual void size_available(const Size& s);

    /**
     * Get the size available to the widget.
     *
     * Used by container widgets
     *
     * @return size available to the widget.
     */
    const Size& size_available() const;

    /**
     * Notification of size change of a child.
     *
     * This is called on a parent widget when the child detects a
     * size change.
     *
     * If the called class is not a container, it should ignore a
     * call.
     *
     * @return the parent returns @c true if the size change can
     * be fullfilled, else @c false.
     */
    virtual bool size_change() = 0;

    /**
     * The size the widget effectively occupies.
     *
     * If the Widget returns a size of Size(0,0) it indicates that
     * it dynamically adjusts to the size_available(). It then
     * means, that the parent Widget has to provide a sensible
     * value for size_available().
     *
     * size_hint() may provide a hint for the size required.
     *
     * @return size the widget effectively occupies, or Size(0,0)
     * if it dynamically adjusts to the size_available().
     */
    virtual Size size() const = 0;

    /**
     * Provide a size hint.
     *
     * Dynamically size Widgets can provide size hints if
     * possible. However, there is no guarantee that they get size
     * they hinted.
     *
     * @return Size::zero() if no hint can be provided. Either
     * Size::rows()>0 or Size::cols()>0 which is taken as
     * hint. WidgetBase::size_hint() == WidgetBase::size() for
     * non-dynamically sized Widgets.
     */
    virtual Size size_hint() const = 0;

    /**
     * Reset the size of the widget.
     *
     * This is used mainly for dynamically sized Widgets, so that
     * will return Size(0,0) upon next call to size().
     *
     * Non-dynamically sized Widget are supposed to ignore this.
     */
    virtual void reset_size() = 0;

    /**
     * Indicates whether or not the Widget is capable of
     * focusing.
     *
     * @return Widgets not capable of focusing return @c false,
     * otherwise @c true.
     */
    virtual bool can_focus() const;

    /**
     * Set or unset the focus to the Widget.
     *
     * If a Widget is not capable of focusing, it must throw an
     * CannotFocus exception when trying to set the focus.
     *
     * focus() must not refresh() the Widget. The FocusGroup takes
     * care of that. Else refreshing of overlapped windows/widgets
     * might get confused.
     *
     * @param f @c true if the Widget has focus, @c false
     * otherwise.
     */
    virtual void focus(bool f);

    /**
     * Get the focus status.
     *
     * Widgets that cannot have focus, must always return @c
     * false.
     *
     * @return @c true if the widget has focus, @c false
     * otherwise.
     */
    virtual bool focus() const;

    // From Realizable
    void unrealize();
};
}  // namespace YACURS

#endif  // WIDGETBASE_H
