#include <stdexcept>

#include "curswin.h"
#include "yacursex.h"

using namespace YACURS::INTERNAL;
//
// Private
//

//
// Protected
//
CursWin::CursWin(WINDOW* win): __window(win),
			       __box(false) {
    if (win==0)
	throw std::invalid_argument("argument 'win' must not be 0");

    int x, y;
    getbegyx(__window, y, x);
    __area.x(x);
    __area.y(y);

    getmaxyx(__window, y, x);
    __area.cols(x);
    __area.rows(y);

    __client_area=__area;
}

//
// Public
//
CursWin::CursWin(const Area& _a): __window(0),
				  __box(false),
				  __subwin(false) {

    if (_a == Area::zero()) {
	throw std::invalid_argument("Area must not be zero");
    }

    __area=__client_area=_a;

    if ( (__window=newwin(__area.rows(),
			  __area.cols(),
			  __area.x(),
			  __area.y()))==0 )
	throw CursesException("newwin");
}

CursWin::CursWin(const CursWin& cw): __window(dupwin(cw.__window)),
				     __box(cw.__box),
				     __subwin(cw.__subwin),
				     __area(cw.__area),
				     __client_area(cw.__client_area) {
    if (__window==0)
	throw CursesException("dupwin");
}

CursWin& CursWin::operator=(const CursWin& cw) {
    if (this==&cw) return *this;

    if (__window!=0) {
	if (delwin(__window)==ERR)
	    throw CursesException("delwin");

	__window=0;
    }

    __window=cw.__window;

    __box=cw.__box;
    __subwin=cw.__subwin;
    __area=cw.__area;
    __client_area=cw.__client_area;

    return *this;
}

CursWin::~CursWin() {
    if (__window!=0)
	if(delwin(__window)==ERR)
	    throw CursesException("delwin");
}

const Area&
CursWin::area() const {
    return __area;
}

const Area&
CursWin::client_area() const {
    return __client_area;
}

bool
CursWin::created() const {
    return __window!=0;
}

CursWin&
CursWin::refresh(bool immediate) {
    if (immediate) {
	if (wrefresh(__window)==ERR)
	    throw CursesException("wrefresh");
    } else {
	if (wnoutrefresh(__window)==ERR)
	    throw CursesException("wnoutrefresh");
    }
    return *this;
}

bool
CursWin::issubwin() const {
    return __subwin;
}

bool
CursWin::has_box() const {
    return __box;
}

CursWin&
CursWin::box(chtype verch, chtype horch) {
    if (::box(__window, verch, horch)==ERR)
	throw CursesException("box");

    __box=true;
    __client_area-=Margin(1,1,1,1);

    return *this;
}

CursWin&
CursWin::unset_box() {
    __client_area=__area;
    __box=false;

    return *this;
}

Coordinates
CursWin::get_cursor() const {
    int y, x;
    getyx(__window, y, x);

    return Coordinates(x,y);
}

CursWin&
CursWin::move(const Coordinates& pos) {
    if (wmove(__window, pos.y(), pos.x())==ERR)
	throw CursesException("wmove");

    return *this;
}

CursWin&
CursWin::clear() {
    if (wclear(__window)==ERR)
	throw CursesException("wclear");

    return *this;
}

CursWin&
CursWin::erase() {
    if (werase(__window)==ERR)
	throw CursesException("werase");

    return *this;
}

CursWin&
CursWin::touch() {
    if (touchwin(__window)==ERR)
	throw CursesException("touchwin");

    return *this;
}

CursWin&
CursWin::untouch() {
    if (untouchwin(__window)==ERR)
	throw CursesException("untouchwin");

    return *this;
}

bool
CursWin::is_touched() const {
    return is_wintouched(__window)==TRUE;
}

CursWin& 
CursWin::addstr(const CurStr& str) {
    if (mymvwaddstr(__window,
		    str.position().y(),
		    str.position().x(), str.c_str())==ERR)
	throw CursesException("mvwaddstr");

    return *this;
}

CursWin& 
CursWin::addstr(const std::string& str) {
    if (mywaddstr(__window, str.c_str())==ERR)
	throw CursesException("waddstr");

    return *this;
}

CursWin& 
CursWin::addstrx(const CurStr& str) {
    // space available to string
    int space=__client_area.cols()-str.position().x()+(__box?1:0);

    if (space==1) {
	if (__box) {
	    // mvaddch advances cursor, but does not move characters
	    // under cursor, which is ideal when placing character
	    // rigtht next to the border.
	    mvaddch(str.position(), '>');
	} else {
	    // does not advance cursor, but moves everything under it
	    // to the right, which is ideal when having no box. If
	    // insch() would be used with box, the vert box char right
	    // next to the cursor is moved off the window.
	    mvinsch(str.position(), '>');
	}
	return *this;
    }

    if (space<1) {
	return *this;
    }

    if (space<str.length()) {
	addnstr(str, space-1).addch('>');
    } else {
	addstr(str);
    }

    return *this;
}

CursWin& 
CursWin::addstrx(const std::string& str) {
    return addstrx(CurStr(str, get_cursor()));
}

CursWin& 
CursWin::addnstr(const CurStr& str, int n) {
    if (mymvwaddnstr(__window, 
		     str.position().y(),
		     str.position().x(),
		     str.c_str(), n)==ERR)
	throw CursesException("mvwaddnstr");

    return *this;
}

CursWin& 
CursWin::addnstr(const std::string& str, int n) {
    if (mywaddnstr(__window, str.c_str(), n)==ERR)
	throw CursesException("waddnstr");

    return *this;
}

CursWin& 
CursWin::addch(const chtype ch) {
    Coordinates end(__area.cols()-1,
		    __area.rows()-1);

    if (get_cursor()==end) {
	(void)waddch(__window, ch);
    } else {
	if (waddch(__window, ch)==ERR)
	    throw CursesException("waddch");
    }

    return *this;
}

CursWin& 
CursWin::mvaddch(const Coordinates& pos, const chtype ch) {
    Coordinates end(__area.cols()-1,
		    __area.rows()-1);

    if (pos==end) {
	(void)mvwaddch(__window, pos.y(), pos.x(), ch);
    } else {
	if (mvwaddch(__window, pos.y(), pos.x(), ch)==ERR)
	    throw CursesException("mvwaddch");
    }
    return *this;
}

CursWin&
CursWin::insch(const chtype ch) {
    if (winsch(__window,ch)==ERR)
	throw CursesException("winsch");

    return *this;
}

CursWin&
CursWin::mvinsch(const Coordinates& pos, const chtype ch) {
    if (mvwinsch(__window, pos.y(), pos.x(), ch)==ERR)
	throw CursesException("mvwinsch");
    
    return *this;
}

CursWin&
CursWin::mvdelch(const Coordinates& pos) {
    if (mvwdelch(__window, pos.y(), pos.x())==ERR)
	throw CursesException("mvwdelch");
    
    return *this;
}

CursWin&
CursWin::delch() {
    if (wdelch(__window)==ERR)
	throw CursesException("wdelch");
    
    return *this;
}

CursWin*
CursWin::derwin(const Area& a) const {
    CursWin* tmp=new CursWin(::derwin(__window, a.rows(), a.cols(), a.y(), a.x()));
    tmp->__subwin=true;

    return tmp;
}

CursWin*
CursWin::subwin(const Area& a) const {
    CursWin* tmp=new CursWin(::subwin(__window, a.rows(), a.cols(), a.y(), a.x()));
    tmp->__subwin=true;

    return tmp;
}

CursWin&
CursWin::operator+=(const CurStr& str) {
    return addstrx(str);
}

CursWin&
CursWin::operator+=(const std::string& str) {
    return addstrx(str);
}

CursWin&
CursWin::operator<<(const CurStr& str) {
    return addstr(str);
}

CursWin&
CursWin::operator<<(const std::string& str) {
    return addstr(str);
}