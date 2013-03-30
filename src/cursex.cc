// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "cursex.h"

BaseCurEx::BaseCurEx(const char* m) : msg(m) {}

BaseCurEx::BaseCurEx(const std::string& m): msg(m) {}

BaseCurEx::~BaseCurEx() throw() {}

const char*
BaseCurEx::what() const throw() { return msg.c_str(); }

CursesException::CursesException(const char* cfct):
    BaseCurEx(std::string(cfct) + std::string("() failed.")) {}

UnableToInitialize::UnableToInitialize() :
    BaseCurEx("unable to initialize curses") {}

NotInitialized::NotInitialized() :
    BaseCurEx("curses not initialized") {}

AlreadyInitialized::AlreadyInitialized() :
    BaseCurEx("curses has already been initialized") {}

AlreadyRealized::AlreadyRealized():
    BaseCurEx("object already realized") {}

NotRealized::NotRealized():
    BaseCurEx("object not realized") {}

SystemError::SystemError(int _errno):
    BaseCurEx(std::strerror(_errno)),
    __errno(_errno) {}

int
SystemError::errorno() const { return __errno; }

WinSizeInvalid::WinSizeInvalid():
    BaseCurEx("TIOCGWINSZ info invalid") {}

UnableToGetWinSize::UnableToGetWinSize():
    BaseCurEx("unable to get window size") {}

CannotFocus::CannotFocus():
    BaseCurEx("Cannot focus") {}

UnexpectedEvent::UnexpectedEvent():
    BaseCurEx("Unexpected event received") {}

ColorsNotInitialized::ColorsNotInitialized():
    BaseCurEx("Colors not initialized") {}

AreaExceeded::AreaExceeded():
    BaseCurEx("An object exceeded space constraints") {}

NotSupported::NotSupported():
    BaseCurEx("Operation not supported") {}

InvalidDialogType::InvalidDialogType():
    BaseCurEx("Invalid dialog type") {}
