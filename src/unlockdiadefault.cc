// $Id$

#include <cassert>

#include "unlockdiadefault.h"

//
// Private
//
UnlockDialogDefault& 
UnlockDialogDefault::operator=(const UnlockDialogDefault&) {
    throw NotSupported();
    return *this;
}

//
// Protected
//
UnlockDialogDefault::UnlockDialogDefault(const std::string& _secret):
    UnlockDialog("Unlock Screen"),
    __secret(_secret),
    __vpack(0),
    __text(0),
    __secret_input(0) {
    __vpack=new VPack;
    __vpack->always_dynamic(true);
    __text=new Label("Please enter password in order to unlock screen");
    __secret_input=new Input<>;
    __secret_input->obscure_input(true);
    
    __vpack->add_back(__text);
    __vpack->add_back(__secret_input);
    widget(__vpack);
}

UnlockDialogDefault::~UnlockDialogDefault() {
    assert(__vpack!=0);
    assert(__text!=0);
    assert(__secret_input!=0);

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
