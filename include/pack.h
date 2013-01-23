// -*- mode: c++ -*-
//
// $Id$

#ifndef PACK_H
#define PACK_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIST
#include <list>
#endif // HAVE_LIST

#include "area.h"
#include "widgetbase.h"

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
 *
 * @see VPack
 * @see HPack
 */
class Pack: public WidgetBase {
    private:
	/**
	 * The Size of the Pack.
	 *
	 * This will be set by calling size_available().
	 *
	 * @internal the only reason for maintaining that field, is so
	 * that we can reset the size when resetsize() is
	 * called. Pack::size() may even choose to use a different
	 * size.
	 */
	Size __size;

	/**
	 * When hinting is enabled (@c true) Pack::size_hint() queries
	 * associated Widgets for hints. When disabled (@c false)
	 * Pack::size_hint() always returns Size::zero().
	 */
	bool __hinting;

	/**
	 * Call curseswindow() on all associated Widgets.
	 */
	void set_all_curseswindow();

	/**
	 * Calculate size required.
	 *
	 * Calculate the size required. Returns a Size not equal to
	 * Size::zero() only if there are no dynamically sized Widgets
	 * associated.
	 *
	 * @internal the idea is to use this function in size(), so it
	 * should return a Size != Size::zero() only if we can
	 * determine the size of all Widgets definitely, without
	 * calculating the size for dynamically sized Widgets. In
	 * other words, the Pack appears non-dynamically only if there
	 * are no dynamically sized Widgets associated.
	 */
	virtual Size calc_size() const = 0;

    protected:
	/**
	 * List of all Widgets associated with the Pack.
	 */
	std::list<WidgetBase*> widget_list;

	void unrealize();

    public:
	Pack();
	~Pack();
	Pack(const Pack& _p);
	const Pack& operator=(const Pack& _p);

	/**
	 * Add a widget to the Pack. The widget will be added in front
	 * of any other widget.
	 *
	 * @param _w the widget to add. The pack does not create a
	 * copy, so the pointer has to remain valid for the lifetime
	 * of the pack.
	 */
	void add_front(WidgetBase* _w);

	/**
	 * Add a widget to the Pack. The widget will be added after
	 * all other widget.
	 *
	 * @param _w the widget to add. The pack does not create a
	 * copy, so the pointer has to remain valid for the lifetime
	 * of the pack.
	 */
	void add_back(WidgetBase* _w);

	/**
	 * Remove a previously added widget.
	 *
	 * @param _w widget to be removed. Pack compares the address
	 * of the widget.
	 */
	void remove(WidgetBase* _w);

	/**
	 * Set the curses window of the pack and all associated widgets.
	 *
	 * Pack has to maintain its curses window pointer as well as the
	 * pointer of the associated widgets.
	 *
	 * @param _p pointer to curses window
	 */
	void curseswindow(WINDOW* _p);

	/**
	 * Set the available size.
	 *
	 * Since Pack is dynamically sizeable, we have to react on
	 * size_available(), unlike statically sized Widgets.
	 *
	 * @param _s the size available to the Pack.
	 */
	void size_available(const Size& _s);

	Size size() const;

	Size size_hint() const;

	/**
	 * Enable/disable hinting mode.
	 *
	 * When hinting is disabled, size_hint() will always return
	 * Size::zero(), else it uses size_hint() of associated
	 * Widgets.
	 *
	 * @param _h @c true enables hinting, @c false disables
	 * hinting.
	 */
	void hinting(bool _h);

	/**
	 * Query hinting mode.
	 *
	 * @return @c true if hinting is enabled, @c false otherwise.
	 *
	 * @sa hinting()
	 */
	bool hinting() const;

	void resetsize();

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
	bool sizechange();

	/**
	 * Refresh all Widgets in the Pack.
	 *
	 * @param immediate @see Realizeable
	 */
	void refresh(bool immediate);

	/**
	 * Resize the pack.
	 *
	 * This function relies on the implementation of realize() of derived
	 * classes.
	 *
	 * @param _s the new size available to the pack.
	 */
	void resize(const Area& _a);

	//
	// Has to be implemented in derrived classes
	//void realize();
};

#endif // PACK_H
