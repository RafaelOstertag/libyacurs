// -*- mode: c++ -*-

#ifndef CURSTR_H
#define CURSTR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "coordinates.h"

namespace YACURS {
    namespace INTERNAL {

	class CurStr : public std::string {
	    private:
		Coordinates __position;

	    public:
		CurStr(const std::string& str,
		       const Coordinates& pos=Coordinates::zero());

		CurStr(const std::string& str, int y, int x);

		CurStr(const CurStr& cs);

		CurStr& operator=(const CurStr& cs);

		const Coordinates& position() const;
	};
    } // namespace INTERNAL
} // namespace YACURS



#endif // CURSTR_H

