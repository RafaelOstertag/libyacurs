// $Id$

#include <cassert>
#include <cstdlib>

#include "unlockdialog.h"

#include "focusmanager.h"
#include "eventqueue.h"
#include "yacursex.h"

using namespace YACURS;

//
// Private
//
UnlockDialog&
UnlockDialog::operator=(const UnlockDialog&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//

//
// Public
//

UnlockDialog::UnlockDialog(const std::string& _title): 
    Dialog(_title, OKCANCEL, AUTOMATIC) {
}

UnlockDialog::~UnlockDialog() {
}

void
UnlockDialog::realize() {
    REALIZE_ENTER;

    Dialog::realize();

    FocusManager::reset();

    REALIZE_LEAVE;
}
