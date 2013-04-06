// $Id$

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdexcept>
#include <vector>

#include "colorparser.h"

using namespace YACURS;

/*
  Color abbrevs:
  --------------
  
  COLOR_BLACK  : k
  COLOR_RED    : r
  COLOR_GREEN  : g
  COLOR_YELLOW : y
  COLOR_BLUE   : b
  COLOR_MAGENTA: m
  COLOR_CYAN   : c
  COLOR_WHITE  : w


  Attr abbrevs:
  -------------

  A_NORMAL     : 0
  A_STANDOUT   : 1
  A_UNDERLINE  : 2
  A_REVERSE    : 3
  A_BLINK      : 4
  A_DIM        : 5
  A_PROTECT    : 6
  A_INVIS      : 7


  Objects:
  --------

  DEFAULT            : DEF
  MESSAGEBOX_TITLE   : MBT
  MESSAGEBOX         : MBX
  INPUTWIDGET_NOFOCUS: IWN
  INPUTWIDGET_FOCUS  : IWF
  INPUTWIDGET_HIDDEN : IWH
  BUTTON_NOFOCUS     : BTN
  BUTTON_FOCUS       : BTF
  LISTBOX            : LBX
  LISTBOX_HILITE     : LBH
  CHECKBOXGROUP      : CBG
  CHECKBOXGROUP_TITLE: CBT


  Default String:
  ---------------

  "DEF:wk0;MBT:cy0;MBX:yc0;IWN:yw3;IWF:yg3;IWH:yy7;BTN:wk0;BTF:kg3;LBX:wk0;LBH:kg3;CBG:yc0;CBT:cy0"

*/

//
// Private
//
const std::string ColorParser::__default_colors("DEF:wk0;MBT:cy0;MBX:yc0;IWN:yw3;IWF:yg3;IWH:yy7;BTN:wk0;BTF:kg3;LBX:wk0;LBH:kg3;CBG:yc0;CBT:cy0");

const std::string&
ColorParser::default_colors() {
    return __default_colors;
}

std::vector<std::string>
ColorParser::tokenize(const std::string& str) const {
    std::string::size_type pos;
    std::vector<std::string> retval(0);

    std::string tmpstr(str);
    while((pos=tmpstr.find(';'))!=std::string::npos) {
	retval.push_back(tmpstr.substr(0,pos));
	tmpstr.erase(0,pos+1);
    }

    // Sort in remaining parts
    if (tmpstr.length()==7)
	retval.push_back(tmpstr);

    return retval;
}

CursColor
ColorParser::process_token(const std::string& tkn) {
    std::string::size_type pos = tkn.find(':');
    if (pos==std::string::npos)
	throw std::invalid_argument("'" + tkn + " does not contain ':'");

    std::string col_obj=tkn.substr(0,pos);
    if (col_obj.length()!=3)
	throw std::invalid_argument("Color Object size invalid");

    std::string colors=tkn.substr(pos+1,tkn.length()-pos);
    if (colors.length()!=3)
	throw std::invalid_argument("Color size invalid");

    CursColor retval;
    if (color_name_map.find(col_obj)==color_name_map.end())
        throw std::out_of_range(col_obj + " is not valid color object");
    retval.no=color_name_map[col_obj];
    
    if (curs_colors_map.find(colors[0])==curs_colors_map.end())
        throw std::out_of_range(colors[0] + " is not a valid color");
    retval.fg=curs_colors_map[colors[0]];
    
    if (curs_colors_map.find(colors[1])==curs_colors_map.end())
        throw std::out_of_range(colors[1] + " is not a valid color");
    retval.bg=curs_colors_map[colors[1]];
    
    if (curs_attrs_map.find(colors[2])==curs_attrs_map.end())
        throw std::out_of_range(colors[2] + " is not a valid attribute");
    retval.attr=curs_attrs_map[colors[2]];

    return retval;
}

std::vector<CursColor>
ColorParser::get_default_scheme() {
    std::vector<std::string> default_tokens(tokenize(default_colors()));

    std::vector<CursColor> retval(NUMBER_OF_COLORS);

    std::vector<std::string>::iterator it=default_tokens.begin();
    while (it!=default_tokens.end()) {
	// We don't catch exceptions, since there must be none. The
	// default color scheme has to be flawless.
	CursColor tmp(process_token(*it++));
	retval.at(tmp.no)=tmp;
    }

    return retval;
}
    

//
// Protected
//

//
// Public
//
ColorParser::ColorParser() {
    color_name_map["DEF"]=DEFAULT;
    color_name_map["MBT"]=MESSAGEBOX_TITLE;
    color_name_map["MBX"]=MESSAGEBOX;
    color_name_map["IWN"]=INPUTWIDGET_NOFOCUS;
    color_name_map["IWF"]=INPUTWIDGET_FOCUS;
    color_name_map["IWH"]=INPUTWIDGET_HIDDEN;
    color_name_map["BTN"]=BUTTON_NOFOCUS;
    color_name_map["BTF"]=BUTTON_FOCUS;
    color_name_map["LBX"]=LISTBOX;
    color_name_map["LBH"]=LISTBOX_HILITE;
    color_name_map["CBG"]=CHECKBOXGROUP;
    color_name_map["CBT"]=CHECKBOXGROUP_TITLE;

    curs_colors_map['k']=COLOR_BLACK;
    curs_colors_map['r']=COLOR_RED;
    curs_colors_map['g']=COLOR_GREEN;
    curs_colors_map['y']=COLOR_YELLOW;
    curs_colors_map['b']=COLOR_BLUE;
    curs_colors_map['m']=COLOR_MAGENTA;
    curs_colors_map['c']=COLOR_CYAN;
    curs_colors_map['w']=COLOR_WHITE;

    curs_attrs_map['0']=A_NORMAL;
    curs_attrs_map['1']=A_STANDOUT;
    curs_attrs_map['2']=A_UNDERLINE;
    curs_attrs_map['3']=A_REVERSE;
    curs_attrs_map['4']=A_BLINK;
    curs_attrs_map['5']=A_DIM;
    curs_attrs_map['6']=A_PROTECT;
    curs_attrs_map['7']=A_INVIS;
}

ColorParser::ColorParser(const ColorParser& cp): color_name_map(cp.color_name_map),
						 curs_colors_map(cp.curs_colors_map),
						 curs_attrs_map(cp.curs_attrs_map) {}

ColorParser& 
ColorParser::operator=(const ColorParser& cp) {
    if (this==&cp) return *this;

    color_name_map=cp.color_name_map;
    curs_colors_map=cp.curs_colors_map;
    curs_attrs_map=cp.curs_attrs_map;
    
    return *this;
}

std::vector<CursColor>
ColorParser::operator()(const std::string& colorstr) {
    std::vector<CursColor> default_scheme(get_default_scheme());

    std::vector<std::string> tokens(tokenize(colorstr));

    std::vector<CursColor> retval(NUMBER_OF_COLORS);
    if (tokens.size()>0) {
	std::vector<std::string>::iterator it=tokens.begin();
	while (it!=tokens.end()) {
	    try {
		CursColor tmp(process_token(*it++));
		retval.at(tmp.no)=tmp;
	    } catch (std::out_of_range&) {
		// ignore
	    }
	}

	// Merge with default colors, so unspecified colors will be
	// initialized.
	for (int i=0; i<NUMBER_OF_COLORS; i++) {
	    if (retval.at(i).no==-1)
		retval.at(i)=default_scheme.at(i);
	}
    } else {
	return default_scheme;
    }

    return retval;
}
