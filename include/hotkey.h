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

#ifndef HOTKEY_H
#define HOTKEY_H 1

namespace YACURS {
class HotKey {
   private:
    int _key;

   public:
    HotKey(int k);

    virtual ~HotKey();

    int key() const;

    virtual void action() = 0;

    virtual HotKey* clone() const = 0;
};
}  // namespace YACURS

#endif  // HOTKEY_H
