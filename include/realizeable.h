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
 * Realization states that can be assumed by an object.
 */
enum REALIZATION_STATE {
    REALIZING,
    REALIZED,
    UNREALIZING,
    UNREALIZED
};

/**
 * Realizeable defines a basic interfaces for displaying and resizing
 * Curses objects on the screen.
 *
 * Realizeable classes display objects on the screen, or provide a way
 * for other objects to be displayed (see Pack).
 *
 * Realizing consists of two states
 *
 * - REALIZING: the object is in the course of being realized, its
 *     state might not be stable
 *
 * - REALIZED: the object is realized. Its state is stable
 *
 *
 * Unrealizing consists of two state
 *
 * - UNREALIZING: the object is in the course of being unrealized, its
 *     state might not be stable
 *
 * - UNREALIZED: the object is unrealized. Its state is stable.
 *
 * Please keep in mind, the implementation has to set the proper states.
 */
class Realizeable {
    private:
	/**
	 * The realization state of the object.
	 */
	REALIZATION_STATE __realization_state;

	/**
	 * Internal variable for realize_enter()/realize_leave()
	 *
	 * Variable is used to keep track of the realization state of
	 * the object when realize_enter() was called. Depending on
	 * the value, a call to realize_leave() will either set
	 * __realization_state to REALIZED (if @false) or not touch
	 * __realization_state (if @c true).
	 */
	bool ___on_realize_enter_was_realizing;

    protected:
	/**
	 * Set realization state.
	 *
	 * @param _s state of the object.
	 */
	virtual void realization(REALIZATION_STATE _s);

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
	 * Get the realization state of the object.
	 *
	 * @return the realization state.
	 */
	REALIZATION_STATE realization() const;
};

/*
 * This macro should be used when implementing realize(). It has to be
 * called first. It will ensure that the REALIZATION_STATE is set
 * properly when REALIZE_LEAVE is called.
 *
 * The main purpose this macro exists, is that derrived classes don't
 * have to rewrite the logic for maintaining proper realization
 * state. For instance, the following pseudo code should always yield
 * the proper realization state, regardless whether or not
 * A::realize() or B::realize() is called, i.e. as long as the first
 * call to realize() is not finished, the state has to be REALIZING.
 *
 *
 *  void A::realize() {
 *    REALIZE_ENTER
 *
 *    // Object realization_state() == REALIZING
 *
 *    REALIZE_LEAVE
 *  }
 *
 *  void B::realize() {
 *    REALIZE_ENTER
 *
 *    A::realize();
 *
 *    // Object realization_state() == REALIZING
 *
 *    REALIZE_LEAVE
 */
#define REALIZE_ENTER (bool __was_realizing_on_enter__=false;		\
		       switch (realization()) {				\
		       REALIZED:					\
		       UNREALIZING:					\
			   return;					\
		       UNREALIZED:					\
			   realizing(REALIZING);			\
			   break;					\
		       REALIZING:					\
			   __was_realizing_on_enter__=true;		\
			   break;					\
		       default:						\
			   assert(0);					\
			   break;					\
		       })

/*
 * Helper function to be used in realize()
 *
 * Has to be the last call in realize(). It ensures that the
 * realization state is set properly.
 */
#define REALIZE_LEAVE (if(!__was_realizing_on_enter__)	\
			   realizing(REALIZED);)

/*
 * The same purpose as REALIZE_ENTER.
 */
#define UNREALIZE_ENTER (bool __was_unrealizing_on_enter__=false;	\
			 switch (realization()) {			\
			 REALIZED:					\
			     realizing(UNREALIZING);			\
			     break;					\
			 UNREALIZING:					\
			     __was_unrealizing_on_enter=true;		\
			     break;					\
			 UNREALIZED:					\
			 REALIZING:					\
			     return;					\
			 default:					\
			     assert(0);					\
			     break;					\
			 })
/*
 * The same purpose as REALIZE_LEAVE.
 */
#define UNREALIZE_LEAVE (if(!__was_unrealizing_on_enter__) \
			     realizing(UNREALIZED);)

#endif // REALIZEABLE_H
