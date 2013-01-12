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
    BaseCurEx("Unable to initialize curses") {}

NotInitialized::NotInitialized() :
    BaseCurEx("Curses not initialized") {}

EndWinError::EndWinError() :
    BaseCurEx("Endwin() had an error") {}

AlreadyInitialized::AlreadyInitialized() :
    BaseCurEx("Curses has already been initialized") {}

RefreshFailed::RefreshFailed() :
    BaseCurEx("Refresh failed") {}

NewWindowFailed::NewWindowFailed() :
    BaseCurEx("Creating new window failed") {}

DelWindowFailed::DelWindowFailed() :
    BaseCurEx("Deleting window failed") {}

BoxFailed::BoxFailed() :
    BaseCurEx("Creating box (border) on window failed") {}

DoupdateFailed::DoupdateFailed() :
    BaseCurEx("doupdate() failed") {}

AddStrFailed::AddStrFailed() :
    BaseCurEx("Adding string to window failed") {}

EraseFailed::EraseFailed() :
    BaseCurEx("Erase of window failed") {}

ScrollOKFailed::ScrollOKFailed() :
    BaseCurEx("call to scrollok() failed") {}

LeaveOKFailed::LeaveOKFailed() :
    BaseCurEx("call to leaveok() failed") {}

NoNLFailed::NoNLFailed() :
    BaseCurEx("call to nonl() failed") {}

AlreadyRealized::AlreadyRealized():
    BaseCurEx("ScreenObject already realized") {}

NotRealized::NotRealized():
    BaseCurEx("ScreenObject is not realized") {}

SystemError::SystemError(int _errno):
    BaseCurEx(strerror(_errno)) {}

WinSizeInvalid::WinSizeInvalid():
    BaseCurEx("TIOCGWINSZ info invalid") {}

UnableToGetWinSize::UnableToGetWinSize():
    BaseCurEx("Unable to get window size") {}

DelCurTermFailed::DelCurTermFailed():
    BaseCurEx("Error in call to del_curterm()") {}

SetupTermFailed::SetupTermFailed():
    BaseCurEx("Error in call to setupterm()") {}

TouchFailed::TouchFailed():
    BaseCurEx("Error in call to setupterm()") {}
