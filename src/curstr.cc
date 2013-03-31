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

CurStr::CurStr(const std::string& str, const Coordinates& pos): 
    std::string(str),
    __position(pos) {
    }

CurStr::CurStr(const std::string& str, int y, int x):
    std::string(str),
    __position(Coordinates(x, y)) {
}
						      
CurStr::CurStr(const CurStr& cs): std::string(cs),
				  __position(cs.__position) {
}

CurStr& CurStr::operator=(const CurStr& cs) {
    if (this==&cs) return *this;

    std::string::operator=(cs);
    __position=cs.__position;

    return *this;
}

const Coordinates&
CurStr::position() const {
    return __position;
}
