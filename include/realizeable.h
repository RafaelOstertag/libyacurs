// -*- mode: c++ -*-
//
// $Id$

#ifndef REALIZEABLE_H
#define REALIZEABLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "area.h"

/**
 * Realizeable defines a basic interfaces for displaying and resizing
 * Curses objects on the screen.
 *
 * Realizeable classes display objects on the screen, or provide a way
 * for other objects to be displayed (see Pack).
 */
class Realizeable {
    private:
	/**
	 * Indicates whether or not the object has been realized.
	 */
	bool __realized;

    protected:
	/**
	 * Set or unset realization state.
	 *
	 * @param _r state of the object.
	 */
	virtual void realized(bool _r);

    public:
	Realizeable();
	Realizeable(const Realizeable& r);
	virtual ~Realizeable();

	Realizeable& operator=(const Realizeable& r);

	/**
	 * Refresh the object. Derrived classes have to implement two
	 * modes:
	 *
	 * - immediate refresh makes changes immediately visible using
         *   Curses wrefresh().
	 * - non-immediate refresh has to prepare for a Curses
         *   doupdate()
	 *
	 * A refresh is supposed make changes to the screen. Adding
	 * text to a curses window, for instance, would be implemented
	 * in a refresh.
	 *
	 * @param immediate @c true indicating an immediate refresh,
	 * @c false indicating non-immediate refresh.
	 */
	virtual void refresh(bool immediate) = 0;

	/**
	 * Resizes the object.
	 *
	 * How the new Area is interpreted depends on the
	 * implementation.
	 *
	 * @internal A resize must not refresh. It is usually a sequence of
	 * unrealize(), realize().
	 *
	 * @param _a the new Area available to the object.
	 */
	virtual void resize(const Area& _a) = 0;

	/**
	 * Realize the object by calling Curses functions and prepare
	 * the object for a refresh.
	 *
	 * Realize is supposed to create curses window and associcated
	 * attributes, such as borders only. It should not be used for
	 * adding text for instance. Adding text and related
	 * operations would be implemented in refresh().
	 */
	virtual void realize() = 0;

	/**
	 * Unrealize the object again.
	 */
	virtual void unrealize() = 0;

	/**
	 * Get the realized state of the object.
	 *
	 * @return @c true if the object is realized, @c false
	 * otherwise.
	 */
	bool realized() const;
};

#endif // REALIZEABLE_H
