// -*- mode: c++ -*-
//
// $Id$

#ifndef FOCUSMANAGER_H
#define FOCUSMANAGER_H

#include <vector>

#include "yacurstypes.h"
#include "focusgroup.h"
#include "widgetbase.h"

namespace YACURS {
    /**
     * @ingroup Focus
     *
     * Focus Manager for Widgets.
     *
     * In Libyacurs, only widgets can be focused. Widgets register
     * themselves upon realization with a Focus Group identified by a
     * Focus Group ID.
     *
     * A Window does create a Focus Group upon realization an receives a
     * Focus Group ID. The Window is then responsible for passing on the
     * Focus Group ID to Widgets in order to let the Widgets register
     * themselves with the proper Focus Group.
     *
     * The Window has to activate the Focus Group upon refresh(), which
     * ensures that the last refreshed Window will have set its Focus
     * Group active, which is supposed to be top most Window, given the
     * EventQueue maintains EventConnectors in the order they were
     * connected.
     *
     * Focus Groups are organized in a vector. Windows are supposed to
     * activate their Focus Group on refresh().
     *
     * The FocusManager is intended to be the only interface to
     * FocusGroups for other objects.
     * 
     * In a nutshell: a Window
     *
     *  - creates a new Focus Group upon realize().
     *  - destroys the Focus Group upon unrealize().
     *  - activates the Focus Group upon refresh().
     *
     * @note Focus Group IDs are zero-based.
     *
     * @sa FocusGroup
     *
     * @internal Early implementations used to maintain a stack and did
     * not employ Focus Group IDs. This works, as long as no screen resize
     * occurs, which can cause Widgets in obscured Windows to unrealize()
     * due to size constraints. Those Widgets would then remove themselves
     * from the Current Focus Group which is not the proper one, since it
     * belongs to the top most Window. So, the implementation now requires
     * to activate the Focus Group on a Window refresh(), since the Window
     * is refreshed before Widgets are, so we maintain activation of
     * proper Focus Group.
     *
     * @internal the Focus Group ID is simply the index of the Focus Group
     * in the vector. We don't remove items from the vector, but set them
     * to @c 0 upon destruction, so that we can identify empty slots.
     */
    class FocusManager {
	private:
	    /**
	     * Holds the Focus Group ID of the Active Focus Group. It is
	     * legal to hold a Focus Group ID of a Group been
	     * removed. focus_group_activate() has to cope with that.
	     */
	    static fgid_t __active_focusgroup;

	    /**
	     * List holding __FocusGroupItem comprised of a Focus Group
	     * and a Focus Group ID.
	     *
	     * Only one Focus Group may be active at a time, which is
	     * always the las created Focus Group, i.e. the Focus Group
	     * associated with the __FocusGroupItem added last.
	     */
	    static std::vector<FocusGroup*> __focus_groups;

	    /**
	     * Handles EVT_FOCUS_NEXT and EVT_FOCUS_PREVIOUS.
	     */
	    static void focus_change_handler(Event& _e);

	public:
	    /**
	     * initializes the Focus Manager.
	     */
	    static void init();

	    /**
	     * uninitializes the Focus Manager.
	     */
	    static void uninit();

	    /**
	     * Create a new Focus Group.
	     *
	     * @return the Focus Group ID to be used when adding/removing
	     * Widgets to the Focus Group.
	     */
	    static fgid_t new_focus_group();

	    /**
	     * Destroy Current Focus Group.
	     *
	     * Destroy Current Focus Group.
	     */
	    static void destroy_focus_group(fgid_t _id);

	    /**
	     * Register Widgets with a Focus Group
	     *
	     * The pointer to the Widget has to be valid for the entire
	     * lifetime of the Current Focus Group.
	     *
	     * @param _id Focus Group ID.
	     *
	     * @param _w pointer to the Widget.
	     */
	    static void focus_group_add(fgid_t _id, WidgetBase* _w);

	    /**
	     * Unregister Widgets from Focus Group
	     *
	     * @param _id Focus Group ID.
	     *
	     * @param _w pointer to the Widget.
	     */
	    static void focus_group_remove(fgid_t _id, WidgetBase* _w);

	    /**
	     * Activate a given Focus Group.
	     *
	     * Activate the Focus Group identified by the ID and
	     * deactivate all other Focus Groups.
	     *
	     * @param _id Focus Group to activate.
	     */
	    static void focus_group_activate(fgid_t _id);

	    /**
	     * Re-focus
	     *
	     * Calls refocus() on Active Focus Group.
	     */
	    static void refocus();

	    /**
	     * Reset the focus on the active Focus Group.
	     */
	    static void reset();
    };
}

#endif // FOCUSMANAGER_H
