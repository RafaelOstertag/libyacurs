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

#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstdlib>
#include <functional>
#include <sstream>
#include <stdexcept>

#include "checkbox.h"
#include "colors.h"
#include "eventqueue.h"
#include "focusmanager.h"
#include "yacursex.h"

using namespace YACURS;

namespace YACURS {
namespace FUNCTORS {
namespace CHECKBOX {
/**
 * Calculate the maximum length of strings
 */
class MaxStrLen {
   private:
    std::string::size_type _max_len;

   public:
    MaxStrLen() : _max_len(0) {}

    std::string::size_type max_len() const { return _max_len; }

    void operator()(const std::string& s) {
#ifdef YACURS_USE_WCHAR
        size_t mbslen = std::mbstowcs(0, s.c_str(), 0);
        if (mbslen == (size_t)-1) throw EXCEPTIONS::SystemError(errno);

        _max_len = std::max(mbslen, _max_len);
#else
        _max_len = std::max(s.length(), _max_len);
#endif
    }
};
}  // namespace CHECKBOX
}  // namespace FUNCTORS
}  // namespace YACURS

//
// Private
//

//
// Protected
//
void CheckBox::key_handler(Event& e) {
    assert(e.type() == EVT_KEY);

    if (!focus()) return;

#ifdef YACURS_USE_WCHAR
    EventEx<wint_t>& ekey = dynamic_cast<EventEx<wint_t>&>(e);
#else
    EventEx<int>& ekey = dynamic_cast<EventEx<int>&>(e);
#endif

    switch (ekey.data()) {
        case KEY_TAB:
            EventQueue::submit(EVT_FOCUS_NEXT);
            break;

        case KEY_ENTER:
        case KEY_RETURN:
        case KEY_RETURN2:
        case ' ':
            set_selection(_cursor);
            EventQueue::submit(
                EventEx<CheckBox*>(EVT_CHECKBOX_SELECTION, this));
            break;

        case KEY_DOWN:
            if (_cursor < _items.size() - 1)
                _cursor++;
            else
                _cursor = 0;
            break;

        case KEY_UP:
            if (_cursor > 0)
                _cursor--;
            else
                _cursor = _items.size() - 1;
            break;

        case KEY_BTAB:
            EventQueue::submit(EVT_FOCUS_PREVIOUS);
            break;
    }

    refresh(true);
}

//
// Public
//

CheckBox::CheckBox(const std::string& title,
                   const std::vector<std::string>& items)
    : _size(Size::zero()), _cursor(0), _title(title) {
    _indicators[0] = "[ ] ";
    _indicators[1] = "[x] ";
    can_focus(true);

    FUNCTORS::CHECKBOX::MaxStrLen len = std::for_each(
        items.begin(), items.end(), FUNCTORS::CHECKBOX::MaxStrLen());

    _items.resize(items.size());
    std::copy(items.begin(), items.end(), _items.begin());

    // +2 because we have a border. +6 because we add the check box
    // indicators and the border.
    _size = Size(_items.size() + 2, len.max_len() + 6);
}

CheckBox::~CheckBox() {
    EventQueue::disconnect_event(
        EventConnectorMethod1<CheckBox>(EVT_KEY, this, &CheckBox::key_handler));
}

bool CheckBox::selected(unsigned short i) {
    if (_items.size() < i) {
        std::ostringstream _ind;
        _ind << i;
        throw std::out_of_range(_("CheckBox: index ") + _ind.str() +
                                _(" out of range."));
    }

    return _items[i].selected;
}

bool CheckBox::selected(const std::string& s) {
    for (std::vector<INTERNAL::Selectable>::size_type n = 0; n < _items.size();
         n++)
        if (_items[n].item == s) return _items[n].selected;

    throw std::out_of_range(_("Item '") + s + _("' not found in CheckBox"));
}

void CheckBox::set_selection(unsigned short cursor) {
    if (cursor >= _items.size())
        throw std::out_of_range(
            _("CheckBox cursor out of range in set_selection()"));

    _items[cursor].selected = !_items[cursor].selected;
}

void CheckBox::set_selection(const std::string& str) {
    std::vector<INTERNAL::Selectable>::size_type n;
    for (n = 0; n < _items.size(); n++)
        if (_items[n].item == str) {
            set_selection(n);
            return;
        }

    throw std::out_of_range(_("Item '") + str + _("' not found in CheckBox"));
}

void CheckBox::size_available(const Size& s) {
    WidgetBase::size_available(s);
}

Size CheckBox::size() const { return _size; }

Size CheckBox::size_hint() const { return _size; }

bool CheckBox::size_change() { return false; }

void CheckBox::reset_size() {
    // Intentionaly empty
}

void CheckBox::refresh(bool immediate) {
    if (realization() != REALIZED) return;
    assert(widget_subwin() != 0);

    std::vector<INTERNAL::Selectable>::iterator it = _items.begin();
    std::vector<INTERNAL::Selectable>::size_type pos = 0;
    std::string item;
    while (++pos, it != _items.end()) {
        item = _indicators[(*it).selected ? 1 : 0] + (*it).item;
        widget_subwin()->addstr(CurStr(item, Coordinates(1, pos)));
        it++;
    }

    if (focus()) {
        widget_subwin()->box(0, 0);
    } else {
        widget_subwin()->box('|', '-');
    }

    if (!_title.empty()) {
        widget_subwin()->addstrx(CurStr(_title, Coordinates(1, 0)));
    }

    if (focus()) {
        curs_set(1);
        widget_subwin()->leaveok(false);
        widget_subwin()->move(Coordinates(2, _cursor + 1));
    } else {
        curs_set(0);
        widget_subwin()->leaveok(true);
    }

    Widget::refresh(immediate);
}

void CheckBox::realize() {
    REALIZE_ENTER;

    Widget::realize();

    EventQueue::connect_event(
        EventConnectorMethod1<CheckBox>(EVT_KEY, this, &CheckBox::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    REALIZE_LEAVE;
}

void CheckBox::unrealize() {
    UNREALIZE_ENTER;
    EventQueue::disconnect_event(
        EventConnectorMethod1<CheckBox>(EVT_KEY, this, &CheckBox::key_handler));

    assert(focusgroup_id() != FocusManager::nfgid);

    Widget::unrealize();
    UNREALIZE_LEAVE;
}
