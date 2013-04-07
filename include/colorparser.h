// -*- c++ -*-
#ifndef COLORPARSER_H
#define COLORPARSER_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <vector>
#include <map>

#include "colors.h"
#include "mycurses.h"

namespace YACURS {
    class ColorParser {
	private:
	    static const std::string __default_colors;

	    std::map<std::string,COLOROBJ> color_name_map;
	    std::map<char,short> curs_colors_map;
	    std::map<char,int> curs_attrs_map;

	    std::vector<std::string> tokenize(const std::string& str) const;
	    CursColor process_token(const std::string& tkn);
	    std::vector<CursColor> get_default_scheme();

	public:
	    ColorParser();
	    ColorParser(const ColorParser& cp);
	    ColorParser& operator=(const ColorParser& cp);
	    std::vector<CursColor> operator()(const std::string& colorstr=default_colors());

	    static const std::string& default_colors();
    };
}

#endif // COLORPARSER_H
