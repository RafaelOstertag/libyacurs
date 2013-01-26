// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cursex.h"

BaseCurEx::BaseCurEx(const char* m) : msg(m) {}

BaseCurEx::BaseCurEx(const BaseCurEx &c) { msg = c.msg; }

BaseCurEx&
BaseCurEx::operator=(const BaseCurEx &c) {
    if (this == &c)
	return *this;

    msg = c.msg;
    return *this;
}

BaseCurEx::~BaseCurEx() throw() {}

const char*
BaseCurEx::what() const throw() { return msg.c_str(); }

UnableToInitialize::UnableToInitialize() :
    BaseCurEx("unable to initialize curses") {}

NotInitialized::NotInitialized() :
    BaseCurEx("curses not initialized") {}

EndWinError::EndWinError() :
    BaseCurEx("Endwin() had an error") {}

AlreadyInitialized::AlreadyInitialized() :
    BaseCurEx("curses has already been initialized") {}

RefreshFailed::RefreshFailed() :
    BaseCurEx("refresh failed") {}

NewWindowFailed::NewWindowFailed() :
    BaseCurEx("creating new window failed") {}

DelWindowFailed::DelWindowFailed() :
    BaseCurEx("deleting window failed") {}

BoxFailed::BoxFailed() :
    BaseCurEx("creating box (border) on window failed") {}

DoupdateFailed::DoupdateFailed() :
    BaseCurEx("doupdate() failed") {}

AddStrFailed::AddStrFailed() :
    BaseCurEx("adding string to window failed") {}

EraseFailed::EraseFailed() :
    BaseCurEx("erase of window failed") {}

ScrollOKFailed::ScrollOKFailed() :
    BaseCurEx("call to scrollok() failed") {}

LeaveOKFailed::LeaveOKFailed() :
    BaseCurEx("call to leaveok() failed") {}

NoNLFailed::NoNLFailed() :
    BaseCurEx("call to nonl() failed") {}

AlreadyRealized::AlreadyRealized():
    BaseCurEx("object already realized") {}

NotRealized::NotRealized():
    BaseCurEx("object not realized") {}

SystemError::SystemError(int _errno):
    BaseCurEx(strerror(_errno)) {}

WinSizeInvalid::WinSizeInvalid():
    BaseCurEx("TIOCGWINSZ info invalid") {}

UnableToGetWinSize::UnableToGetWinSize():
    BaseCurEx("unable to get window size") {}

DelCurTermFailed::DelCurTermFailed():
    BaseCurEx("error in call to del_curterm()") {}

SetupTermFailed::SetupTermFailed():
    BaseCurEx("error in call to setupterm()") {}

TouchFailed::TouchFailed():
    BaseCurEx("error in call to setupterm()") {}

CbreakFailed::CbreakFailed():
    BaseCurEx("error in call to setupterm()") {}

NoEchoFailed::NoEchoFailed():
    BaseCurEx("error in call to setupterm()") {}

DerWinFailed::DerWinFailed():
    BaseCurEx("derwin() failed") {}

ClearFailed::ClearFailed():
    BaseCurEx("*clear() failed") {}

CannotFocus::CannotFocus():
    BaseCurEx("Cannot focus") {}

KeyPadFailed::KeyPadFailed():
    BaseCurEx("keypad() failed") {}

UnexpectedEvent::UnexpectedEvent():
    BaseCurEx("Unexpected event received") {}
