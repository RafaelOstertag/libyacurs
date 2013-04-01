#include <stdexcept>

#include "curswin.h"
#include "yacursex.h"

using namespace YACURS::INTERNAL;
//
// Private
//
CursWin::CursWin(const CursWin& cw) {
    throw NotSupported();
}

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
CursWin::create() {
    if (__window!=0) return *this;

    if ( (__window=newwin(__area.rows(),
			  __area.cols(),
			  __area.x(),
			  __area.y()))==0 )
	throw CursesException("newwin");

    return *this;
}

void
CursWin::destroy() {
    if (__window==0) return;

    if (delwin(__window)==ERR)
	throw CursesException("delwin");

    __window=0;
    __client_area=__area=Area::zero();
}

CursWin&
CursWin::refresh(bool immediate) {
    if (__window==0) return *this;

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
    if (__window==0) return *this;

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
    if (__window==0) return Coordinates();

    int y, x;
    getyx(__window, y, x);

    return Coordinates(x,y);
}

CursWin&
CursWin::move(const Coordinates& pos) {
    if (__window==0) return *this;

    int y=__client_area.y()-__area.y()+pos.y();
    int x=__client_area.x()-__area.x()+pos.x();

    if (wmove(__window, y, x)==ERR)
	throw CursesException("wmove");

    return *this;
}

CursWin&
CursWin::clear() {
    if (__window==0) return *this;

    if (wclear(__window)==ERR)
	throw CursesException("wclear");

    return *this;
}

CursWin&
CursWin::erase() {
    if (__window==0) return *this;

    if (werase(__window)==ERR)
	throw CursesException("werase");

    return *this;
}

CursWin&
CursWin::touch() {
    if (__window==0) return *this;

    if (touchwin(__window)==ERR)
	throw CursesException("touchwin");

    return *this;
}

CursWin&
CursWin::untouch() {
    if (__window==0) return *this;

    if (untouchwin(__window)==ERR)
	throw CursesException("untouchwin");

    return *this;
}

bool
CursWin::is_touched() const {
    if (__window==0) return false;

    return is_wintouched(__window)==TRUE;
}

CursWin& 
CursWin::addstr(const CurStr& str) {
    if (__window==0) return *this;

    int y=__client_area.y()-__area.y()+str.position().y();
    int x=__client_area.x()-__area.x()+str.position().x();

    if (mymvwaddstr(__window, y, x, str.c_str())==ERR)
	throw CursesException("mvwaddstr");

    return *this;
}

CursWin& 
CursWin::addstr(const std::string& str) {
    if (__window==0) return *this;

    if (mywaddstr(__window, str.c_str())==ERR)
	throw CursesException("waddstr");

    return *this;
}

CursWin& 
CursWin::addstrx(const CurStr& str) {
    if (__window==0) return *this;

    if (!(__client_area>str.position()))
	throw std::invalid_argument("Position outside of area");

    int x=__client_area.x()-__area.x()+str.position().x();

    // space available to string with box taken into account
    int space=std::min(__client_area.cols(),__area.cols())-x;

    if (space<str.length()) {
	addnstr(str, space-1).addch('>');
    } else {
	addstr(str);
    }

    return *this;
}

CursWin& 
CursWin::addstrx(const std::string& str) {
    if (__window==0) return *this;

    Coordinates tmp(get_cursor());

    int space=std::min(__client_area.cols(),__area.cols())-tmp.x();

    if (space<str.length()) {
	addnstr(str, space-1).addch('>');
    } else {
	addstr(str);
    }

    return *this;
}

CursWin& 
CursWin::addnstr(const CurStr& str, int n) {
    if (__window==0) return *this;

    int y=__client_area.y()-__area.y()+str.position().y();
    int x=__client_area.x()-__area.x()+str.position().x();

    if (mymvwaddnstr(__window, y, x, str.c_str(), n)==ERR)
	throw CursesException("mvwaddnstr");

    return *this;
}

CursWin& 
CursWin::addnstr(const std::string& str, int n) {
    if (__window==0) return *this;

    if (mywaddnstr(__window, str.c_str(), n)==ERR)
	throw CursesException("waddnstr");

    return *this;
}

CursWin& 
CursWin::addch(const chtype ch) {
    if (__window==0) return *this;

    if (waddch(__window, ch)==ERR)
	throw CursesException("waddch");

    return *this;
}

CursWin& 
CursWin::mvaddch(const chtype ch, const Coordinates& pos) {
    if (__window==0) return *this;

    int y=__client_area.y()-__area.y()+pos.y();
    int x=__client_area.x()-__area.x()+pos.x();
    
    if (mvwaddch(__window, y, x, ch)==ERR)
	throw CursesException("mvwaddch");
    
    return *this;
}

CursWin*
CursWin::derwin(const Area& a) const {
    if (__window==0) return 0;

    CursWin* tmp=new CursWin(::derwin(__window, a.rows(), a.cols(), a.y(), a.x()));
    tmp->__subwin=true;

    return tmp;
}

CursWin&
CursWin::operator<<(const CurStr& str) {
    if (__window==0) return *this;

    int y=__client_area.y()-__area.y()+str.position().y();
    int x=__client_area.x()-__area.x()+str.position().x();

    if (mymvwaddstr(__window, y, x, str.c_str())==ERR)
	throw CursesException("mvwaddstr");

    return *this;
}

CursWin&
CursWin::operator<<(const std::string str) {
    if (__window==0) return *this;

    if (mywaddstr(__window, str.c_str())==ERR)
	throw CursesException("waddstr");

    return *this;
}
