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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cerrno>

#include "cursorbuf.h"
#include "yacursex.h"

using namespace YACURS::INTERNAL;
//
// Private
//

//
// Protected
//

//
// Public
//
CursorBuf::CursorBuf(std::wstring _buffer, tsz_t _max_size):
    __buffer(_buffer),
    __curs_pos(0),
    __max_size(_max_size) {}

CursorBuf::CursorBuf(std::string _buffer, tsz_t _max_size):
    __curs_pos(0),
    __max_size(_max_size) {

    wchar_t* tmpbuf = new wchar_t[_buffer.length()+1];

    size_t retval = mbstowcs(tmpbuf, _buffer.c_str(), _buffer.length());
    if (retval == _buffer.length()+1)
	tmpbuf[_buffer.length()]=L'\0';

    if (retval==(size_t)-1) {
	delete[] tmpbuf;
	throw EXCEPTIONS::SystemError(errno);
    }

    __buffer=tmpbuf;
}
	
CursorBuf::CursorBuf(const CursorBuf& _cb):
    __buffer(_cb.__buffer),
    __curs_pos(_cb.__curs_pos),
    __max_size(_cb.__max_size) {}


CursorBuf&
CursorBuf::operator=(const CursorBuf& _cb) {
    if (&_cb == this) return *this;

    __buffer=_cb.__buffer;
    __curs_pos=_cb.__curs_pos;
    __max_size=_cb.__max_size;

    return *this;
}

void
CursorBuf::buffer(std::wstring& _b) {
    __curs_pos=0;
    __buffer = _b;
}

const std::wstring&
CursorBuf::buffer_wstring() const {
    return __buffer;
}

void
CursorBuf::clear_buffer() {
    __buffer.clear();
    __curs_pos = 0;
}

void
CursorBuf::clear_eol() {
    __buffer = __buffer.erase(__curs_pos,
			      __buffer.length() - __curs_pos );
}

void
CursorBuf::backspace() {
    if (__curs_pos == 0) return;

    __curs_pos--;

    if (!__buffer.empty() )
	__buffer = __buffer.erase(__curs_pos, 1);
}

void
CursorBuf::delete_char() {
    if (__curs_pos >= __buffer.length() ||
	__buffer.empty() ) return;
	    
    __buffer = __buffer.erase(__curs_pos, 1);
}

void
CursorBuf::home() {
    __curs_pos=0;
}

void
CursorBuf::end() {
    if (__buffer.empty()) return;
    __curs_pos=__buffer.length()-1;
}

void
CursorBuf::back_step() {
    if (__curs_pos==0) return;

    __curs_pos--;
}

void
CursorBuf::forward_step() {
    if (__curs_pos>=__buffer.length()-1) return;

    __curs_pos++;
}

void
CursorBuf::insert(std::wstring::value_type c) {
    // do not overrun the max size
    if (__buffer.length() >= __max_size) return;

    if (__curs_pos == __buffer.length()-1)
	__buffer.push_back(c);
    else
	__buffer.insert(__curs_pos, c, 1);

    __curs_pos++;
}

std::wstring
CursorBuf::get_wstring(int16_t _size, int16_t* curs_pos) {

    tsz_t offset= (__curs_pos % _size) * _size;
    if (curs_pos!=0)
	(*curs_pos)=static_cast<uint16_t>(__curs_pos - offset);

    return __buffer.substr(offset, (offset + _size>__buffer.length()-1)? __buffer.length()-offset : _size);
}

std::string
CursorBuf::get_string(int16_t _size, int16_t* curs_pos) {
    std::wstring tmp_wbuf=get_wstring(_size, curs_pos);

    size_t mbr_len = wcstombs(0, tmp_wbuf.c_str(), 0);
    char* tmp_mb_buff = new char[mbr_len+1];

    if (wcstombs(tmp_mb_buff, tmp_wbuf.c_str(), mbr_len+1)==(size_t)-1) {
	delete[] tmp_mb_buff;
	throw EXCEPTIONS::SystemError(errno);
    }

    std::string retval(tmp_mb_buff);
    delete[] tmp_mb_buff;

    return retval;
}
