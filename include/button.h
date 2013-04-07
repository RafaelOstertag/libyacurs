// -*- mode: c++ -*-
//
// $Id$

#ifndef BUTTON_H
#define BUTTON_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "label.h"
#include "event.h"

namespace YACURS {
    /**
     * A button that can be pressed.
     *
     * Upon press, a button emits @c EVT_BUTTON_PRESS. All Event
     * Connectors will receive that event. The can distinguish between
     * different buttons by comparing EventEx<Button*>::data() to the
     * address of the Button they're interested in.
     */
    class Button: public Label {
	private:
	    /**
	     * Temporary string used when returning label.
	     */
	    std::string __tmp_label;

	    // Not supported
	    Button& operator=(const Button&);

	protected:
	    /**
	     * Key Handler.
	     *
	     * Handles @c EVT_KEY events.
	     *
	     * @param _e EventEx<int>.
	     */
	    virtual void key_handler(Event& _e);

	    // From Realizeable
	    virtual void realize();
	    virtual void unrealize();
	public:
	    /**
	     * Constructor.
	     *
	     * @param _b label
	     */
	    Button(const std::string& _b);

	    virtual ~Button();

	    /**
	     * Set label.
	     *
	     * @param _l label
	     */
	    void label(const std::string& _l);

	    /**
	     * Get label.
	     *
	     * @return label of Button.
	     */
	    const std::string& label() const;

	    // From Realizeable
	    void refresh(bool immediate);
   };
}

#endif // BUTTON_H
