// -*- c++ -*-
#ifndef COLORPARSER_H
#define COLORPARSER_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <vector>

#include "colors.h"
#include "mycurses.h"

namespace YACURS {
    class ColorParser {
	private:
	    std::string str;

	    std::vector<std::string> split_tokens();
	public:
	    ColorParser(const ColorParser& cp);
	    ColorParser& operator=(const ColorParser& cp);
	    void operator()(const std::string& colorstr);
    };
}

#endif // COLORPARSER_H
