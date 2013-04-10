// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "hotkey.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//

//
// Public
//
HotKey::HotKey(int _k): __key(_k) {}

HotKey::HotKey(const HotKey& hk): __key(hk.__key) {}

HotKey&
HotKey::operator=(const HotKey& hk) {
    __key=hk.__key;
    return *this;
}

HotKey::~HotKey() {}

int
HotKey::key() const {
    return __key;
}
