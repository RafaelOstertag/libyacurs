// -*- mode: c++ -*-
//
// This file is part of libyacurs.
// Copyright (C) 2013  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see
// <http://www.gnu.org/licenses/>.
//
//
// $Id$

#ifndef CURSORBUF_H
#define CURSORBUF_H

#include <string>

namespace YACURS {

    namespace INTERNAL {

	class CursorBuf {
	    public:
		// Convenience
		typedef std::wstring::size_type tsz_t;

	    private:
		/**
		 * The input
		 */
		std::wstring __buffer;

		/**
		 * Cursor position.
		 */
		tsz_t __curs_pos;

		/**
		 * Maximum size of of buffer.
		 *
		 * If it is 0, there is no limit.
		 */
		tsz_t __max_size;

	    public:
		/**
		 * Constructor.
		 *
		 * @param _size display size
		 *
		 * @param _max_size maximum size of the input buffer.
		 *
		 * @param _buffer initial values of the buffer.
		 */
		CursorBuf(std::wstring _buffer=std::wstring(), tsz_t _max_size=255);

		CursorBuf(std::string _buffer=std::string(), tsz_t _max_size=255);

		CursorBuf(const CursorBuf& _cb);
		CursorBuf& operator=(const CursorBuf& _cb);

		void buffer(std::wstring& _b);
		void buffer(std::string& _b);
		const std::wstring& buffer_wstring() const;
		const std::string buffer_string() const;

		void reset_cursor();

		void clear_buffer();
		void clear_eol();
		void backspace();
		void delete_char();
		void home();
		void end();
		void back_step();
		void forward_step();
		void insert(std::wstring::value_type c);

		std::wstring get_wstring(int16_t _size, int16_t* curs_pos);
		std::string  get_string(int16_t _size, int16_t* curs_pos);
	};
    } // namespace INTERNAL
} // namespace YACURS

#endif // CURSORBUF_H
