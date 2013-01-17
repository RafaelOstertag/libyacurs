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
 * objects on the screen.
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
	 * @param immediate @c true indicating an immediate refresh,
	 * @c false indicating non-immediate refresh.
	 */
	virtual void refresh(bool immediate) = 0;
	/**
	 * Resizes the object.
	 *
	 * How the new size is interpreted depends on the
	 * implementation.
	 *
	 * A resize must not refresh.
	 *
	 * @param _r the new size available to the object.
	 */
	virtual void resize(const Area& _r) = 0;
	/**
	 * Realize the object by calling Curses functions and prepare
	 * the object for a refresh.
	 */
	virtual void realize() = 0;
	/**
	 * Get the realized state of the object.
	 *
	 * @return @c true if the object is realized, @c false
	 * otherwise.
	 */
	bool realized() const;
};

#endif // REALIZEABLE_H
