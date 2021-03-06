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

#ifndef PACK_H
#define PACK_H 1

#include <list>

#include "area.h"
#include "widgetbase.h"

namespace YACURS {
/**
 * A container widget for stacking widgets horizontally or vertically.
 *
 * A pack is a list of widgets, that are either horizontally, or
 * vertically stacked and displayed.
 *
 * The pack will calculate the size needed for displaying the widgets
 * horizontally, vertically. This should happen on realize() which is
 * required to be implemented in derived classes.
 *
 * @warning If a Pack is destroyed before its associated Widgets, the
 * Widgets will have a __parent pointer that is invalid.
 *
 * @see VPack
 * @see HPack
 */
class Pack : public WidgetBase {
   private:
    /**
     * The Size of the Pack.
     *
     * This will be set by calling size_available().
     *
     * @internal
     *
     * the only reason for maintaining that field
     * is, so that we can reset the size when reset_size() is
     * called and return Size::zero() on subsequent calls to
     * Pack::size(). Pack::size() may even choose to use a
     * different size (@sa Pack::size()).
     */
    Size _size;

    /**
     * When hinting is enabled (@c true) Pack::size_hint() queries
     * associated Widgets for hints and returns a hint based on
     * the result. When disabled (@c false) Pack::size_hint()
     * always returns Size::zero().
     *
     * Default @c true.
     */
    bool _hinting;

    /**
     * Indicates whether or not the Pack should behave as
     * dynamically sized Widget.
     *
     * If @c true, a call to Pack::size() does NOT invoke call
     * calc_size_non_dynamic(), which in turn returns Size !=
     * Size::zero() if all associated Widgets are non-dynamic.
     *
     * If @c false Pack::size() calls calc_size_non_dynamic() and
     * may return that result.
     *
     * Default @c false.
     */
    bool _always_dynamic;

    /**
     * Call curses_window() on all associated Widgets.
     */
    void set_all_curses_window();

    /**
     * Call focusgroup_id() on all associated Widgets.
     */
    void set_all_focusgroup_id();

    /**
     * Refresh all associated Widgets.
     *
     * @param i immediate flag. @sa Realizeable::refresh().
     */
    void refresh_all_widgets(bool i);

    /**
     * Calculate size required.
     *
     * Calculate the size required. Returns a Size not equal
     * to Size::zero() only if there are no dynamically sized
     * Widgets associated.
     *
     * @internal
     *
     * the idea is to use this function in size(), so it
     * should return a Size != Size::zero() only if we can
     * determine the size of all Widgets definitely, without
     * calculating the size for dynamically sized Widgets. In
     * other words, the Pack appears non-dynamically only if
     * there are no dynamically sized Widgets associated.
     */
    virtual Size calc_size_non_dynamic() const = 0;

    void take_over(WidgetBase* w);

   protected:
    /**
     * List of all Widgets associated with the Pack.
     */
    std::list<WidgetBase*> widget_list;

    void unrealize();

   public:
    Pack();

    Pack& operator=(const Pack&) = delete;
    Pack(const Pack&) = delete;
    Pack& operator=(Pack&&) = delete;
    Pack(Pack&&) = delete;

    virtual ~Pack();

    std::list<WidgetBase*>::size_type widgets() const;

    /**
     * Add a widget to the Pack. The widget will be added in front
     * of any other widget.
     *
     * @important Widgets can only be added before the Pack is
     * realized. It is not possible to add a Widget after the pack
     * has been realized. Attempt to do so will raise
     * AlreadyRealized exception
     *
     * @param w the widget to add. The pack does not create a
     * copy, so the pointer has to remain valid for the lifetime
     * of the pack.
     */
    void add_front(WidgetBase* w);

    /**
     * Add a widget to the Pack. The widget will be added after
     * all other widget.
     *
     * @important Widgets can only be added before the Pack is
     * realized. It is not possible to add a Widget after the pack
     * has been realized. Attempt to do so will raise
     * AlreadyRealized exception
     *
     * @param w the widget to add. The pack does not create a
     * copy, so the pointer has to remain valid for the lifetime
     * of the pack.
     */
    void add_back(WidgetBase* w);

    /**
     * Remove a previously added widget.
     *
     * @important Widgets can only be added before the Pack is
     * realized. It is not possible to add a Widget after the pack
     * has been realized. Attempt to do so will raise
     * AlreadyRealized exception
     *
     * @param w widget to be removed. Pack compares the address
     * of the widget.
     */
    void remove(WidgetBase* w);

    /**
     * Set the curses window of the Pack and all associated
     * Widgets.
     *
     * Pack has to maintain its curses window pointer as well as
     * the pointer of the associated widgets.
     *
     * @param p pointer to curses window
     */
    void curses_window(YACURS::INTERNAL::CursWin* p);

    /**
     * Set the Focus Group ID of the Pack and all associated
     * Widgets.
     *
     * @param id Focus Group ID.
     */
    void focusgroup_id(FocusManager::fgid_t id);

    /**
     * Set the available size.
     *
     * Since Pack is dynamically sizeable, we have to react on
     * size_available(), unlike statically sized Widgets.
     *
     * @param s the size available to the Pack.
     */
    void size_available(const Size& s);

    Size size() const;

    /**
     * Enable/disable hinting mode.
     *
     * When hinting is disabled, size_hint() will always return
     * Size::zero(), else it uses size_hint() of associated
     * Widgets.
     *
     * By default, hinting is enabled.
     *
     * @param h @c true enables hinting, @c false disables
     * hinting.
     */
    void hinting(bool h);

    /**
     * Query hinting mode.
     *
     * @return @c true if hinting is enabled, @c false otherwise.
     *
     * @sa hinting()
     */
    bool hinting() const;

    /**
     * Enable/disable always dynamic mode.
     *
     * If always dynamic is disabled, the pack will behave like a
     * non-dynamic Widget, if, and only if, it does not contain
     * any dynamicially sized Widgets. Else it behaves like a
     * dynamic Widget.
     *
     * If always dynmic is enabled, it behaves always like a
     * dynamic widget.
     *
     * By default, always dynamic is disabled.
     *
     * @param d @c true to enable always dynamic mode, @c false
     * to disable it.
     */
    void always_dynamic(bool d);

    /**
     * Query always dynamic mode.
     *
     * @return @c true if always dynamic mode is enabled, @c false
     * otherwise.
     */
    bool always_dynamic() const;

    void reset_size();

    /**
     * Handle a size change in an associated Widget.
     *
     * If the pack has a parent, the notification is passed
     * further up. Else it will handle the notification by calling
     * Pack::resize().
     *
     * @return @c true if the notification can be handled, @c
     * false otherwise.
     */
    bool size_change();

    bool can_focus() const;

    void focus(bool);

    bool focus() const;

    /**
     * Refresh all Widgets in the Pack.
     *
     * @param immediate @see Realizeable
     */
    void refresh(bool immediate);

    /**
     * Resize the pack.
     *
     * This function relies on the implementation of realize() of
     * derived classes.
     *
     * @param a the new size available to the pack.
     */
    void resize(const Area& a);

    //
    // Has to be implemented in derrived classes
    // void realize();
};
}  // namespace YACURS

#endif  // PACK_H
