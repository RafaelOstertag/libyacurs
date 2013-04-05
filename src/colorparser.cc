o#ifdef HAVE_CONFIG_H
# include "config.h"
#endif



#include <vector>

#include "colorparser.cc"

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

  "DEF:wy0;MBT:cy0;MBX:yc0;IWN:yw3;IWF:yg3;IWH:yy7;BTN:wy0;BTF:yg3;LBX:wy0;LBH:yg3;CBG:yc0;CBT:cy0"

*/

ColorParser::ColorParser(const ColorParser& cp): str(cp.str) {}

ColorParser& 
ColorParser::operator=(const ColorParser& cp) {
str=cp.str;
}

void
ColorParser::operator()(std::string colorstr) {

}
