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

#ifndef TITLEBAR_H
#define TITLEBAR_H 1

#include "lineobject.h"

namespace YACURS {
class TitleBar : public LineObject {
   public:
    TitleBar(POSITION pos, const std::string& t);
    TitleBar& operator=(const TitleBar&) = delete;
    TitleBar& operator=(TitleBar&&) = delete;
    TitleBar(const TitleBar&) = delete;
    TitleBar(TitleBar&&) = delete;
};
}  // namespace YACURS

#endif
