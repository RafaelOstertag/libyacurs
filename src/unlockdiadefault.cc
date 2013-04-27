//
// This file is part of libyacurs,
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
UnlockDialogDefault&
UnlockDialogDefault::operator=(const UnlockDialogDefault&) {
    throw EXCEPTIONS::NotSupported();
    return *this;
}

//
// Protected
//
UnlockDialogDefault::UnlockDialogDefault(const std::string& _secret) :
    UnlockDialog(_("Unlock Screen") ),
    __secret(_secret),
    __vpack(0),
    __text(0),
    __secret_input(0) {
    __vpack = new VPack;
    __vpack->always_dynamic(true);
    __text = new Label(_("Please enter password in order to unlock screen") );
    __text->color(DIALOG);
    __secret_input = new Input<>;
    __secret_input->obscure_input(true);

    __vpack->add_back(__text);
    __vpack->add_back(__secret_input);
    widget(__vpack);
}

UnlockDialogDefault::~UnlockDialogDefault() {
    assert(__vpack != 0);
    assert(__text != 0);
    assert(__secret_input != 0);

    delete __vpack;
    delete __text;
    delete __secret_input;
}

bool
UnlockDialogDefault::unlock() {
    if (dialog_state() == DIALOG_OK &&
        __secret_input->input() == __secret)
        return true;

    return false;
}

void
UnlockDialogDefault::clear() {
    __secret_input->clear();
}

//
// Public
//
