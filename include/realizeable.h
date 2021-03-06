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

#ifndef REALIZEABLE_H
#define REALIZEABLE_H 1

#include <string>

#include "area.h"

namespace YACURS {
/**
 * Realization states that can be assumed by an object.
 */
enum REALIZATION_STATE { REALIZING, REALIZED, UNREALIZING, UNREALIZED };

/**
 * Realizeable defines a basic interfaces for displaying and resizing
 * Curses objects on the screen.
 *
 * Realizeable classes display objects on the screen, or provide a way
 * for other objects to be displayed (see Pack).
 *
 * The functions
 *
 * - refresh()
 * - realize()
 * - unrealize()
 * - resize()
 *
 * must not rely on being only called when the object is in the proper
 * state. They must handle calls while being in inproper state
 * gracefully, i.e. not throwing exceptions due to an unexpected
 * state. For instance: refresh() must not throw an exception when
 * called on unrealized objects. Another example would be realize()
 * being called twice.
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
 * Please keep in mind, the implementation has to set the proper
 * states. The helper macros @c REALIZE_ENTER and @c REALIZE_LEAVE can
 * be used when entering and leaving realize() function, @c
 * UNREALIZE_ENTER and @c UNREALIZE_LEAVE for unrealize.
 *
 */
class Realizeable {
   private:
    /**
     * The realization state of the object.
     */
    REALIZATION_STATE _realization_state;

   protected:
    /**
     * Set realization state.
     *
     * @param s state of the object.
     */
    virtual void realization(REALIZATION_STATE s);

   public:
    Realizeable();

    virtual ~Realizeable();

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
     * @param _a the new Area available to the object.
     *
     * @internal
     *
     * A resize must not refresh. It is usually a sequence of
     * unrealize(), realize().
     */
    virtual void resize(const Area& _a) = 0;

    /**
     * Realize the object by calling Curses functions and prepare
     * the object for a refresh.
     *
     * Realize is supposed to create curses window. It should not
     * be used for adding text for instance. Adding text and
     * related operations would be implemented in refresh().
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
}  // namespace YACURS

/**
 * @relates YACURS::Realizeable
 *
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
 * <pre>
 *  void A::realize() {
 *    REALIZE_ENTER;
 *
 *    // Object realization_state() == REALIZING
 *
 *    REALIZE_LEAVE;
 *  }
 *
 *  void B::realize() {
 *    REALIZE_ENTER;
 *
 *    A::realize();
 *
 *    // Object realization_state() == REALIZING
 *
 *    REALIZE_LEAVE;
 * </pre>
 */
#define REALIZE_ENTER                          \
    bool __was_realizing_on_enter__ = false;   \
    switch (realization()) {                   \
        case REALIZED:                         \
        case UNREALIZING:                      \
            return;                            \
        case UNREALIZED:                       \
            realization(REALIZING);            \
            break;                             \
        case REALIZING:                        \
            __was_realizing_on_enter__ = true; \
            break;                             \
        default:                               \
            assert(0);                         \
            break;                             \
    }

/**
 * @relates YACURS::Realizeable
 *
 * Helper macro to be used in realize()
 *
 * Has to be the last call in realize(). It ensures that the
 * realization state is set properly.
 *
 * @warning if realize() has to be left due to error condition, do not
 * use REALIZE_LEAVE, instead set realization() to the proper
 * realization state (which, in that case, is most likely @c
 * UNREALIZED).
 */
#define REALIZE_LEAVE \
    if (!__was_realizing_on_enter__) realization(REALIZED);

/**
 * @relates YACURS::Realizeable
 *
 * The same purpose as REALIZE_ENTER.
 */
#define UNREALIZE_ENTER                          \
    bool __was_unrealizing_on_enter__ = false;   \
    switch (realization()) {                     \
        case REALIZED:                           \
            realization(UNREALIZING);            \
            break;                               \
        case UNREALIZING:                        \
            __was_unrealizing_on_enter__ = true; \
            break;                               \
        case UNREALIZED:                         \
        case REALIZING:                          \
            return;                              \
        default:                                 \
            assert(0);                           \
            break;                               \
    }

/**
 * @relates YACURS::Realizeable
 *
 * The same purpose as REALIZE_LEAVE.
 */
#define UNREALIZE_LEAVE \
    if (!__was_unrealizing_on_enter__) realization(UNREALIZED);

#endif  // REALIZEABLE_H
