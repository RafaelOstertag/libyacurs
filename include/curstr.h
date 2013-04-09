// -*- mode: c++ -*-

#ifndef CURSTR_H
#define CURSTR_H

#include <string>

#include "coordinates.h"
#include "colors.h"

namespace YACURS {
    namespace INTERNAL {

	class CurStr : public std::string {
	    private:
		Coordinates __position;
		COLOROBJ __color;

	    public:
		CurStr(const std::string& str,
		       const Coordinates& pos=Coordinates::zero(),
		       COLOROBJ c=DEFAULT);

		CurStr(const std::string& str, int y, int x, COLOROBJ c=DEFAULT);

		CurStr(const CurStr& cs);

		CurStr& operator=(const CurStr& cs);

		const Coordinates& position() const;
		COLOROBJ color() const;
	};
    } // namespace INTERNAL
} // namespace YACURS



#endif // CURSTR_H

