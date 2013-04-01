// -*- mode: c++ -*-

#ifndef CURSWIN_H
#define CURSWIN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>

#include "mycurses.h"
#include "area.h"
#include "curstr.h"

namespace YACURS {
    namespace INTERNAL {

	/**
	 * Wrapper class for Curses Window.
	 */
	class CursWin {
	    private:
		/**
		 * Pointer to Curses Window.
		 */
		WINDOW* __window;

		/**
		 * Flag whether or not Curses Window has a box.
		 */
		bool __box;

		/**
		 * Flag indicating whether or not this is Subwindow.
		 */
		bool __subwin;

		/**
		 * Size and position of the Window.
		 */
		Area __area;

		/**
		 * Area available to client.
		 */
		Area __client_area;

		/**
		 * Not supported.
		 *
		 * @internal Supporting those would become
		 * unintuitive. Depending on the implementation, for
		 * instance, one copy of an object might set a border,
		 * but all other instances wouldn't know about it, i.e
		 * the Curses Window knows it, but the __box flag
		 * isn't set accordingly. Another approach would be
		 * making all instances aware of such changes, but
		 * then again, why not using references in the first
		 * place?.
		 */
		CursWin(const CursWin& cw);
	    protected:
		CursWin(WINDOW* win);

	    public:
		CursWin(const Area& _a);
		CursWin& operator=(const CursWin& cw);

		~CursWin();

		/**
		 * Size and position of the Window.
		 *
		 * @return size and position of the Window.
		 */
		const Area& area() const;

		/**
		 * Size and position available to clients.
		 *
		 * Size and position available for use.
		 *
		 * @return size and position.
		 */
		const Area& client_area() const;

		/**
		 * Query whether or not Curses Window has been
		 * created.
		 */
		bool created() const;

		/**
		 * Create the Curses Window.
		 */
		CursWin& create();

		/**
		 * Destroy Curses Window.
		 */
		void destroy();

		/**
		 * Refresh Curses Window.
		 *
		 * @param immediate if @c true, call wrefresh() on the Curses
		 * Window, @c false make it call wnoutrefresh().
		 */
		CursWin& refresh(bool immediate=true);

		bool issubwin() const;

		bool has_box() const;

		CursWin& box(chtype verch=0, chtype horch=0);

		CursWin& unset_box();

		Coordinates get_cursor() const;

		CursWin& move(const Coordinates& pos);

		CursWin& clear();

		CursWin& erase();

		CursWin& touch();

		CursWin& untouch();

		bool is_touched() const;

		CursWin& addstr(const CurStr& str);

		CursWin& addstr(const std::string& str);

		CursWin& addstrx(const CurStr& str);

		CursWin& addstrx(const std::string& str);

		CursWin& addnstr(const CurStr& str, int n);

		CursWin& addnstr(const std::string& str, int n);
		
		CursWin& addch(const chtype ch);

		CursWin& mvaddch(const chtype ch, const Coordinates& pos);

		CursWin& insch(const chtype ch);

		CursWin& mvinsch(const chtype ch, const Coordinates& pos);

		CursWin* derwin(const Area& a) const;

		CursWin& operator<<(const CurStr& str);

		CursWin& operator<<(const std::string str);

	};
    } // namespace INTERNAL
} // namespace YACURS



#endif // CURSWIN_H
