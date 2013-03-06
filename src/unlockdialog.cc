// $Id: dialog.cc 4900 2013-03-02 07:35:28Z rafisol $

#include <cassert>
#include <cstdlib>

#include "unlockdialog.h"

#include "eventqueue.h"

//
// Private
//
UnlockDialog&
UnlockDialog::operator=(const UnlockDialog&) {
    abort();
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
