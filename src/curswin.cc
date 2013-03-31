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
			       __references(new int),
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

    *__references=1;
}

//
// Public
//
CursWin::CursWin(const Area& _a): __window(0),
				  __references(new int),
				  __box(false),
				  __subwin(false) {

    if (_a == Area::zero()) {
	delete __references;
	throw std::invalid_argument("Area must not be zero");
    }

    __area=__client_area=_a;

    *__references=1;

}

CursWin::CursWin(const CursWin& cw): __window(cw.__window),
				     __references(cw.__references),
				     __box(cw.__box),
				     __subwin(cw.__subwin),
				     __area(cw.__area),
				     __client_area(cw.__client_area) {
    (*__references)++;
}

CursWin& CursWin::operator=(const CursWin& cw) {
    if (this==&cw) return *this;

    (*__references)--;

    // Make sure we clean up in case we were the last reference to the
    // Curses Window.
    if ( (*__references)<1 &&
	 __window!=0 ) {
	// What should we do if delwin() fails? Throwing an
	// exception leaves an partially initialized window, so we
	// ignore it.
	if (delwin(__window) != ERR)
	    __window=0;
    }

    __window=cw.__window;

    (*(__references=cw.__references))++;

    __box=cw.__box;
    __subwin=cw.__subwin;
    __area=cw.__area;
    __client_area=cw.__client_area;

    return *this;
}

CursWin::~CursWin() {
    (*__references)--;

    if ( (*__references)<1 ) {
	delete __references;

	if (__window!=0)
	    (void)delwin(__window);
    }
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
CursWin::set_box(chtype verch, chtype horch) {
    if (__window==0) return *this;

    if (box(__window, verch, horch)==ERR)
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
CursWin::set_cursor(const Coordinates& pos) {
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

CursWin
CursWin::derwin(const Area& a) const {
    if (__window==0)
	throw NotInitialized();

    CursWin tmp(::derwin(__window, a.rows(), a.cols(), a.y(), a.x()));
    tmp.__subwin=true;

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
