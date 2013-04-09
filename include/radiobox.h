// -*- mode: c++ -*-
//
// $Id: input.h 4875 2013-02-23 13:25:34Z rafisol $

#ifndef RADIOBOX_H
#define RADIOBOX_H

#include "checkbox.h"

namespace YACURS {
    class RadioBox: public CheckBox {
	private:
	    RadioBox& operator=(const RadioBox& _i);
	protected:
	    virtual void set_selection(unsigned short _cursor);

	public:
	    RadioBox(const std::string& _title,
		     const std::vector<std::string>& _items);

	    virtual ~RadioBox();
    };
}

#endif // RADIOBOX_H
