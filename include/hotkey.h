// -*- mode: c++ -*-
//
// $Id$

#ifndef HOTKEY_H
#define HOTKEY_H

namespace YACURS {

    class HotKey {
	private:
	    int __key;

	public:
	    HotKey(int _k);
	    HotKey(const HotKey& hk);
	    HotKey& operator=(const HotKey& hk);
	    virtual ~HotKey();

	    int key() const;

	    virtual void action() = 0;

	    virtual HotKey* clone() const = 0;
    };
}

#endif // HOTKEY_H
