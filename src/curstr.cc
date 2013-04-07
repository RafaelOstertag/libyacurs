#include "curstr.h"

using namespace YACURS::INTERNAL;

//
// Private
//

//
// Protected
//

//
// Public
//

CurStr::CurStr(const std::string& str, const Coordinates& pos, COLOROBJ c): 
    std::string(str),
    __position(pos),
    __color(c) {}

CurStr::CurStr(const std::string& str, int y, int x, COLOROBJ c):
    std::string(str),
    __position(Coordinates(x, y)),
    __color(c) {}
						      
CurStr::CurStr(const CurStr& cs): std::string(cs),
				  __position(cs.__position),
				  __color(cs.__color) {}

CurStr& CurStr::operator=(const CurStr& cs) {
    if (this==&cs) return *this;

    std::string::operator=(cs);
    __position=cs.__position;
    __color=cs.__color;

    return *this;
}

const Coordinates&
CurStr::position() const {
    return __position;
}

YACURS::COLOROBJ
CurStr::color() const {
    return __color;
}
