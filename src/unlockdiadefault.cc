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

#include "unlockdiadefault.h"

using namespace YACURS;

//
// Private
//

//
// Protected
//
UnlockDialogDefault::UnlockDialogDefault(const std::string& secret)
    : UnlockDialog(_("Unlock Screen")),
      _secret(secret),
      _vpack(),
      _text(_("Please enter password in order to unlock screen")),
      _secret_input() {
    _vpack.always_dynamic(true);
    _text.color(DIALOG);
    _secret_input.obscure_input(true);

    _vpack.add_back(&_text);
    _vpack.add_back(&_secret_input);
    widget(&_vpack);
}

UnlockDialogDefault::~UnlockDialogDefault() {}

bool UnlockDialogDefault::unlock() {
    if (dialog_state() == DIALOG_OK && _secret_input.input() == _secret)
        return true;

    return false;
}

void UnlockDialogDefault::clear() { _secret_input.clear(); }

//
// Public
//
