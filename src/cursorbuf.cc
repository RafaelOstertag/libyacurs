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
CursorBuf::CursorBuf(const std::wstring& _buffer, tsz_t _max_size):
    __buffer(_buffer),
    __vcurs_pos(0),
    __max_size(_max_size) {}

CursorBuf::CursorBuf(const std::string& _buffer, tsz_t _max_size):
    __vcurs_pos(0),
    __max_size(_max_size) {

    wchar_t* tmpbuf = new wchar_t[_buffer.length()+1];

    size_t retval = mbstowcs(tmpbuf, _buffer.c_str(), _buffer.length()+1);
    if (retval == _buffer.length()+1)
	tmpbuf[_buffer.length()]=L'\0';

    if (retval==(size_t)-1) {
	delete[] tmpbuf;
	throw EXCEPTIONS::SystemError(errno);
    }

    __buffer=tmpbuf;

    delete[] tmpbuf;
}
	
CursorBuf::CursorBuf(const CursorBuf& _cb):
    __buffer(_cb.__buffer),
    __vcurs_pos(_cb.__vcurs_pos),
    __max_size(_cb.__max_size) {}


CursorBuf&
CursorBuf::operator=(const CursorBuf& _cb) {
    if (&_cb == this) return *this;

    __buffer=_cb.__buffer;
    __vcurs_pos=_cb.__vcurs_pos;
    __max_size=_cb.__max_size;

    return *this;
}

void
CursorBuf::buffer(const std::wstring& _b) {
    __vcurs_pos=0;
    __buffer = _b;
}

void
CursorBuf::buffer(const std::string& _b) {
   wchar_t* tmpbuf = new wchar_t[_b.length()+1];

   size_t retval = mbstowcs(tmpbuf, _b.c_str(), _b.length()+1);
   if (retval == _b.length()+1)
       tmpbuf[_b.length()]=L'\0';

   if (retval==(size_t)-1) {
       delete[] tmpbuf;
       throw EXCEPTIONS::SystemError(errno);
   }

   buffer(tmpbuf);

   delete[] tmpbuf;
}

const std::wstring&
CursorBuf::buffer_wstring() const {
    return __buffer;
}

std::string
CursorBuf::buffer_string() const {
    size_t mbr_len = wcstombs(0, __buffer.c_str(), 0);
    char* tmp_mb_buff = new char[mbr_len+1];

    if (wcstombs(tmp_mb_buff, __buffer.c_str(), mbr_len+1)==(size_t)-1) {
	delete[] tmp_mb_buff;
	throw EXCEPTIONS::SystemError(errno);
    }

    std::string retval(tmp_mb_buff);
    delete[] tmp_mb_buff;

    return retval;
}

void
CursorBuf::clear_buffer() {
    __buffer.clear();
    __vcurs_pos = 0;
}

void
CursorBuf::clear_eol() {
    if (__buffer.empty()) return;

    __buffer = __buffer.erase(__vcurs_pos,
			      __buffer.length() - __vcurs_pos );
}

void
CursorBuf::backspace() {
    if (__vcurs_pos == 0 || __buffer.empty()) return;

    __vcurs_pos--;

    __buffer = __buffer.erase(__vcurs_pos, 1);
}

void
CursorBuf::delete_char() {
    if (__vcurs_pos >= __buffer.length() ||
	__buffer.empty() ) return;
	    
    __buffer = __buffer.erase(__vcurs_pos, 1);
}

void
CursorBuf::home() {
    __vcurs_pos=0;
}

void
CursorBuf::end() {
    if (__buffer.empty()) return;

    __vcurs_pos=__buffer.length();
}

void
CursorBuf::back_step() {
    if (__vcurs_pos==0 || __buffer.empty()) return;

    __vcurs_pos--;
}

void
CursorBuf::forward_step() {
    if (__buffer.empty() || __vcurs_pos>=__buffer.length()) return;

    __vcurs_pos++;
}

void
CursorBuf::insert(std::wstring::value_type c) {
    // do not overrun the max size
    if (__buffer.length() >= __max_size) return;

    if (__vcurs_pos == __buffer.length())
	__buffer.push_back(c);
    else
	__buffer.insert(__vcurs_pos, c, 1);

    __vcurs_pos++;
}

std::wstring
CursorBuf::get_wstring(int16_t _size, int16_t* curs_pos) {

    tsz_t offset= (__vcurs_pos / _size) * _size;
    if (offset>1)
    	(*curs_pos)=static_cast<uint16_t>(__vcurs_pos - offset);
    else
    	(*curs_pos)=static_cast<uint16_t>(__vcurs_pos);

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
