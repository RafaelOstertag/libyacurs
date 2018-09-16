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

#include "gettext.h"

#include <cassert>
#include <cerrno>
#include <stdexcept>

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
CursorBuffer::CursorBuffer(const std::wstring& buffer, tsz_t max_size)
    : _buffer{buffer.length() > max_size ? buffer.substr(0, max_size) : buffer},
      _mbs_cache{},
      _mbs_cache_valid{false},
      _changed{false},
      _curs_pos{0},
      _offset{0},
      _max_size{max_size} {}

CursorBuffer::CursorBuffer(const std::string& buffer, tsz_t max_size)
    : _mbs_cache{},
      _mbs_cache_valid{false},
      _changed{false},
      _curs_pos{0},
      _offset{0},
      _max_size{max_size} {
    wchar_t* tmpbuf = new wchar_t[buffer.length() + 1];

    size_t len = buffer.length() > _max_size ? _max_size : buffer.length();

    size_t retval = std::mbstowcs(tmpbuf, buffer.c_str(), len + 1);

    if (retval == len + 1) tmpbuf[len] = L'\0';

    if (retval == (size_t)-1) {
        delete[] tmpbuf;
        throw EXCEPTIONS::SystemError(errno);
    }

    _buffer = tmpbuf;

    delete[] tmpbuf;
}

CursorBuffer::CursorBuffer(const CursorBuffer& cb)
    : _buffer{cb._buffer},
      _mbs_cache{cb._mbs_cache},
      _mbs_cache_valid{cb._mbs_cache_valid},
      _changed{cb._changed},
      _curs_pos{cb._curs_pos},
      _offset{cb._offset},
      _max_size{cb._max_size} {}

CursorBuffer::CursorBuffer(CursorBuffer&& cb)
    : _buffer{std::move(cb._buffer)},
      _mbs_cache{cb._mbs_cache},
      _mbs_cache_valid{cb._mbs_cache_valid},
      _changed{cb._changed},
      _curs_pos{cb._curs_pos},
      _offset{cb._offset},
      _max_size{cb._max_size} {}

CursorBuffer& CursorBuffer::operator=(const CursorBuffer& cb) {
    if (&cb == this) return *this;

    _buffer = cb._buffer;
    _mbs_cache = cb._mbs_cache;
    _mbs_cache_valid = cb._mbs_cache_valid;
    _changed = cb._changed;
    _curs_pos = cb._curs_pos;
    _offset = cb._offset;
    _max_size = cb._max_size;

    return *this;
}

CursorBuffer& CursorBuffer::operator=(CursorBuffer&& cb) {
    if (&cb == this) return *this;

    _buffer = std::move(cb._buffer);
    _mbs_cache = cb._mbs_cache;
    _mbs_cache_valid = cb._mbs_cache_valid;
    _changed = cb._changed;
    _curs_pos = cb._curs_pos;
    _offset = cb._offset;
    _max_size = cb._max_size;

    return *this;
}

void CursorBuffer::set(const std::wstring& buffer) {
    // Do nothing if the string is the same as the one we already have
    if (buffer == _buffer) return;

    _curs_pos = 0;
    _offset = 0;
    if (buffer.length() > _max_size)
        _buffer = buffer.substr(0, _max_size);
    else
        _buffer = buffer;

    _mbs_cache_valid = false;
    _changed = false;
}

void CursorBuffer::set(const std::string& buffer) {
    wchar_t* tmpbuf = new wchar_t[buffer.length() + 1];

    size_t retval = std::mbstowcs(tmpbuf, buffer.c_str(), buffer.length() + 1);

    if (retval == buffer.length() + 1) tmpbuf[buffer.length()] = L'\0';

    if (retval == (size_t)-1) {
        delete[] tmpbuf;
        throw EXCEPTIONS::SystemError(errno);
    }

    set(tmpbuf);

    delete[] tmpbuf;
}

CursorBuffer::tsz_t CursorBuffer::length() const { return _buffer.length(); }

void CursorBuffer::max_size(CursorBuffer::tsz_t max_size) {
    _max_size = max_size;

    // Re-set buffer, so it will be truncated.
    if (_buffer.length() > max_size) {
        set(_buffer.substr(0, max_size));
        // set() does not set _changed state.  Since we know, the
        // string has been changed, we set it here.
        _changed = true;

        // reset scrolling information
        _curs_pos = 0;
        _offset = 0;
    }
}

CursorBuffer::tsz_t CursorBuffer::max_size() const { return _max_size; }

CursorBuffer::tsz_t CursorBuffer::get_cursor() const { return _curs_pos; }

void CursorBuffer::clear() {
    _buffer.clear();
    _changed = true;
    _curs_pos = 0;
    _mbs_cache_valid = false;
}

void CursorBuffer::clear_eol() {
    if (_buffer.empty()) return;
    tsz_t __vcurs_pos = _offset + _curs_pos;
    _buffer = _buffer.erase(__vcurs_pos, _buffer.length() - __vcurs_pos);

    _mbs_cache_valid = false;
    _changed = true;
}

void CursorBuffer::clear_sol() {
    if (_buffer.empty() || _curs_pos == 0) return;
    tsz_t __vcurs_pos = _offset + _curs_pos;
    _buffer = _buffer.erase(0, __vcurs_pos);

    _curs_pos = 0;
    _offset = 0;

    _mbs_cache_valid = false;
    _changed = true;
}

void CursorBuffer::backspace() {
    if (_curs_pos == 0 || _buffer.empty()) return;

    _curs_pos--;

    tsz_t __vcurs_pos = _offset + _curs_pos;

    _buffer = _buffer.erase(__vcurs_pos, 1);

    _mbs_cache_valid = false;
    _changed = true;
}

void CursorBuffer::del() {
    tsz_t __vcurs_pos = _offset + _curs_pos;
    if (__vcurs_pos >= _buffer.length() || _buffer.empty()) return;

    _buffer = _buffer.erase(__vcurs_pos, 1);

    _mbs_cache_valid = false;
    _changed = true;
}

void CursorBuffer::home() {
    _curs_pos = 0;
    _offset = 0;
}

void CursorBuffer::end() {
    if (_buffer.empty()) return;
    _curs_pos = _buffer.length();
}

void CursorBuffer::back_step() {
    if (_curs_pos == 0 || _buffer.empty()) return;

    _curs_pos--;
}

void CursorBuffer::forward_step() {
    if (_buffer.empty() || _offset + _curs_pos >= _buffer.length()) return;

    _curs_pos++;
}

void CursorBuffer::insert(std::wstring::value_type c) {
    // do not overrun the max size
    if (_buffer.length() >= _max_size) return;

    _mbs_cache_valid = false;
    _changed = true;

    tsz_t __vcurs_pos = _offset + _curs_pos;

    if (__vcurs_pos == _buffer.length())
        _buffer.push_back(c);
    else
        _buffer.insert(__vcurs_pos, 1, c);

    _curs_pos++;
}

/**
 *
 * Not sure fi *len is correct, but *curs_pos should fit...
 *
 */
void CursorBuffer::info(int16_t _size, int16_t* len, int16_t* curs_pos) const {
    if (_size < 2) throw std::out_of_range(_("_size must not be <2"));

    if (curs_pos) *curs_pos = _curs_pos;

    if (len) {
        if (_buffer.empty())
            *len = 0;
        else
            *len =
                static_cast<uint16_t>(((_offset + _size > _buffer.length() - 1)
                                           ? _buffer.length() - _offset
                                           : _size) -
                                      _offset);
    }
}

/**
 * @author Markus Neumann
 *
 * @internal
 *
 * This implementation shows propper scrolling.
 *
 */
std::wstring CursorBuffer::wstring(int16_t _size, int16_t* curs_pos) {
    if (_size < 2) throw std::out_of_range(_("_size must not be <2"));

    /**
     *
     * _offset needs to be changed, if _curs_pos exceeds window limit.
     *
     */
    if (_curs_pos >= _size) {
        _offset = _offset + _curs_pos - _size + 1;
        _curs_pos = _size - 1;
    }
    /**
     *
     * display always 1char on the left, to see what you delete.
     *
     */
    if (_curs_pos <= 0) {
        if (_offset > 0) {
            _offset = _offset + _curs_pos - 1;
            _curs_pos = 1;
        }
    }

    if (curs_pos) *curs_pos = _curs_pos;

    return _buffer.substr(_offset, (_offset + _size > _buffer.length() - 1)
                                       ? _buffer.length() - _offset
                                       : _size);
}

std::string CursorBuffer::string(int16_t _size, int16_t* curs_pos) {
    std::wstring tmp_wbuf = wstring(_size, curs_pos);

    size_t mbr_len = std::wcstombs(0, tmp_wbuf.c_str(), 0);
    char* tmp_mb_buff = new char[mbr_len + 1];

    if (std::wcstombs(tmp_mb_buff, tmp_wbuf.c_str(), mbr_len + 1) ==
        (size_t)-1) {
        delete[] tmp_mb_buff;
        throw EXCEPTIONS::SystemError(errno);
    }

    std::string retval(tmp_mb_buff);
    delete[] tmp_mb_buff;

    return retval;
}

const std::wstring& CursorBuffer::wstring() const { return _buffer; }

const std::string& CursorBuffer::string() const {
    if (_mbs_cache_valid) return _mbs_cache;

    size_t mbr_len = std::wcstombs(0, _buffer.c_str(), 0);
    char* tmp_mb_buff = new char[mbr_len + 1];

    if (std::wcstombs(tmp_mb_buff, _buffer.c_str(), mbr_len + 1) ==
        (size_t)-1) {
        delete[] tmp_mb_buff;
        throw EXCEPTIONS::SystemError(errno);
    }

    _mbs_cache = tmp_mb_buff;
    _mbs_cache_valid = true;

    delete[] tmp_mb_buff;

    return _mbs_cache;
}

bool CursorBuffer::changed() const { return _changed; }
