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
CursorBuffer::CursorBuffer(const std::wstring& _buffer, tsz_t _max_size) :
    __buffer(_buffer),
    __mbs_cache(),
    __mbs_cache_valid(false),
    __vcurs_pos(0),
    __max_size(_max_size) {
}

CursorBuffer::CursorBuffer(const std::string& _buffer, tsz_t _max_size) :
    __mbs_cache(),
    __mbs_cache_valid(false),
    __vcurs_pos(0),
    __max_size(_max_size) {
    wchar_t* tmpbuf = new wchar_t[_buffer.length() + 1];

    size_t retval = mbstowcs(tmpbuf, _buffer.c_str(), _buffer.length() + 1);

    if (retval == _buffer.length() + 1)
        tmpbuf[_buffer.length()] = L'\0';

    if (retval == (size_t)-1) {
        delete[] tmpbuf;
        throw EXCEPTIONS::SystemError(errno);
    }

    __buffer = tmpbuf;

    delete[] tmpbuf;
}

CursorBuffer::CursorBuffer(const CursorBuffer& _cb) :
    __buffer(_cb.__buffer),
    __mbs_cache(_cb.__mbs_cache),
    __mbs_cache_valid(_cb.__mbs_cache_valid), 
    __vcurs_pos(_cb.__vcurs_pos),
    __max_size(_cb.__max_size) {
}

CursorBuffer&
CursorBuffer::operator=(const CursorBuffer& _cb) {
    if (&_cb == this) return *this;

    __buffer = _cb.__buffer;
    __mbs_cache = _cb.__mbs_cache;
    __mbs_cache_valid = _cb.__mbs_cache_valid;
    __vcurs_pos = _cb.__vcurs_pos;
    __max_size = _cb.__max_size;

    return *this;
}

void
CursorBuffer::set(const std::wstring& _b) {
    __vcurs_pos = 0;
    if (_b.length()>__max_size)
	__buffer = _b.substr(0, __max_size-1);
    else
	__buffer = _b;

    __mbs_cache_valid=false;
}

void
CursorBuffer::set(const std::string& _b) {
    wchar_t* tmpbuf = new wchar_t[_b.length() + 1];

    size_t retval = mbstowcs(tmpbuf, _b.c_str(), _b.length() + 1);

    if (retval == _b.length() + 1)
        tmpbuf[_b.length()] = L'\0';

    if (retval == (size_t)-1) {
        delete[] tmpbuf;
        throw EXCEPTIONS::SystemError(errno);
    }

    set(tmpbuf);

    delete[] tmpbuf;
}

CursorBuffer::tsz_t
CursorBuffer::length() const {
    return __buffer.length();
}

void
CursorBuffer::reset_cursor() {
    __vcurs_pos = 0;
}

CursorBuffer::tsz_t
CursorBuffer::get_cursor() const {
    return __vcurs_pos;
}

void
CursorBuffer::clear() {
    __buffer.clear();
    __vcurs_pos = 0;
    __mbs_cache_valid=false;
}

void
CursorBuffer::clear_eol() {
    if (__buffer.empty() ) return;

    __buffer = __buffer.erase(__vcurs_pos,
                              __buffer.length() - __vcurs_pos);

    __mbs_cache_valid=false;
}

void
CursorBuffer::backspace() {
    if (__vcurs_pos == 0 || __buffer.empty() ) return;

    __vcurs_pos--;

    __buffer = __buffer.erase(__vcurs_pos, 1);

    __mbs_cache_valid=false;
}

void
CursorBuffer::del() {
    if (__vcurs_pos >= __buffer.length() ||
        __buffer.empty() ) return;

    __buffer = __buffer.erase(__vcurs_pos, 1);

    __mbs_cache_valid=false;
}

void
CursorBuffer::home() {
    __vcurs_pos = 0;
}

void
CursorBuffer::end() {
    if (__buffer.empty() ) return;

    __vcurs_pos = __buffer.length();
}

void
CursorBuffer::back_step() {
    if (__vcurs_pos == 0 || __buffer.empty() ) return;

    __vcurs_pos--;
}

void
CursorBuffer::forward_step() {
    if (__buffer.empty() || __vcurs_pos >= __buffer.length() ) return;

    __vcurs_pos++;
}

void
CursorBuffer::insert(std::wstring::value_type c) {
    // do not overrun the max size
    if (__buffer.length() >= __max_size) return;

    __mbs_cache_valid=false;

    if (__vcurs_pos == __buffer.length() )
        __buffer.push_back(c);
    else
        __buffer.insert(__vcurs_pos, 1, c);

    __vcurs_pos++;
}

std::wstring
CursorBuffer::wstring(int16_t _size, int16_t* curs_pos) const {
    tsz_t offset = (__vcurs_pos / _size) * _size;

    if (offset > 1)
        (*curs_pos) = static_cast<uint16_t>(__vcurs_pos - offset);
    else
        (*curs_pos) = static_cast<uint16_t>(__vcurs_pos);

    return __buffer.substr(offset,
                           (offset + _size > __buffer.length() -
         1) ? __buffer.length() - offset : _size);
}

std::string
CursorBuffer::string(int16_t _size, int16_t* curs_pos) const {
    std::wstring tmp_wbuf = wstring(_size, curs_pos);

    size_t mbr_len = wcstombs(0, tmp_wbuf.c_str(), 0);
    char* tmp_mb_buff = new char[mbr_len + 1];

    if (wcstombs(tmp_mb_buff, tmp_wbuf.c_str(), mbr_len + 1) == (size_t)-1) {
        delete[] tmp_mb_buff;
        throw EXCEPTIONS::SystemError(errno);
    }

    std::string retval(tmp_mb_buff);
    delete[] tmp_mb_buff;

    return retval;
}

const std::wstring&
CursorBuffer::wstring() const {
    return __buffer;
}

const std::string&
CursorBuffer::string() const {
    if (__mbs_cache_valid) return __mbs_cache;

    size_t mbr_len = wcstombs(0, __buffer.c_str(), 0);
    char* tmp_mb_buff = new char[mbr_len + 1];

    if (wcstombs(tmp_mb_buff, __buffer.c_str(), mbr_len + 1) == (size_t)-1) {
        delete[] tmp_mb_buff;
        throw EXCEPTIONS::SystemError(errno);
    }

    __mbs_cache = tmp_mb_buff;
    __mbs_cache_valid=true;

    delete[] tmp_mb_buff;

    return __mbs_cache;
}
