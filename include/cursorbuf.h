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

#include <stdint.h>

#include <string>
#include <cstdlib>

namespace YACURS {
    namespace INTERNAL {
        class CursorBuffer {
            public:

                // Convenience
                typedef std::wstring::size_type tsz_t;

            private:
                /**
                 * The input
                 */
                std::wstring __buffer;

		/**
		 * Cache
		 *
		 * Holds __buffer converted to multibyte
		 * string. Reference returned by string().
		 */
		mutable std::string __mbs_cache;

		/**
		 * Cache Valid Flag
		 *
		 * Flag indicating whether or not __mbs_cache is
		 * valid, or has to be re-calculated.
		 */
		mutable bool __mbs_cache_valid;

                /**
                 * Cursor position.
                 */
                tsz_t __vcurs_pos;

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
                CursorBuffer(
                    const std::wstring& _buffer=std::wstring(), tsz_t _max_size=
                        255);

                CursorBuffer(
                    const std::string& _buffer=std::string(), tsz_t _max_size=
                        255);

                CursorBuffer(const CursorBuffer& _cb);
                CursorBuffer& operator=(const CursorBuffer& _cb);

		/**
		 * Set new buffer content.
		 */
                void set(const std::wstring& _b);

		/**
		 * Set new buffer content.
		 */
                void set(const std::string& _b);

		/**
		 * Get length of buffer.
		 */
		tsz_t length() const;

		/**
		 * Reset the virtual cursor position
		 */
                void reset_cursor();

		/**
		 * Get the current cursor position
		 */
		tsz_t get_cursor() const;

		/**
		 * Clear the content of the buffer
		 */
                void clear();

		/**
		 * Clear buffer starting from the current cursor
		 * position to the end.
		 */
                void clear_eol();

		/**
		 * Backspace from the current cursor position.
		 */
                void backspace();

		/**
		 * Delete character at current cursor position.
		 */
                void del();

		/**
		 * Move cursor to beginning of buffer.
		 */
                void home();

		/**
		 * Move cursor to end of buffer.
		 */
                void end();

		/**
		 * Move cursor one character towards the beginning of
		 * the buffer.
		 */
                void back_step();

		/**
		 * Move cursor one character towards the end of the
		 * buffer.
		 */
                void forward_step();

		/**
		 * Insert one character at the current cursor
		 * position.
		 */
                void insert(std::wstring::value_type c);

		std::wstring wstring(int16_t _size, int16_t* curs_pos) const;

                std::string  string(int16_t _size, int16_t* curs_pos) const;

		const std::wstring& wstring() const;
		
                const std::string& string() const;
        };
    } // namespace INTERNAL
} // namespace YACURS

#endif // CURSORBUF_H
