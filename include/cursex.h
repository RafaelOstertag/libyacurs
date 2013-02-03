// -*- mode: c++ -*-

#ifndef CUREX_H
#define CUREX_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CSTRING
#include <cstring>
#endif // HAVE_CSTRING

#ifdef HAVE_EXCEPTION
#include <exception>
#endif // HAVE_EXCEPTION

#ifdef HAVE_STRING
#include <string>
#endif // HAVE_STRING

/**
 * Base class from which all libyacurs exceptions are derived.
 */
class BaseCurEx: public std::exception {
    private:
	/**
	 * The message of the exception.
	 */
	std::string msg;

    public:
	BaseCurEx(const char* m);
	BaseCurEx(const BaseCurEx &c);
	BaseCurEx& operator=(const BaseCurEx &c);
	virtual ~BaseCurEx() throw();

	/**
	 * Show the error message.
	 *
	 * @return error message.
	 */
	virtual const char* what() const throw();
};

class UnableToInitialize: public BaseCurEx {
    public:
	UnableToInitialize();
};

class NotInitialized: public BaseCurEx {
    public:
	NotInitialized();
};

class EndWinError: public BaseCurEx {
    public:
	EndWinError();
};

class AlreadyInitialized: public BaseCurEx {
    public:
	AlreadyInitialized();
};

class RefreshFailed: public BaseCurEx {
    public:
	RefreshFailed();
};

class NewWindowFailed: public BaseCurEx {
    public:
	NewWindowFailed();
};

class DelWindowFailed: public BaseCurEx {
    public:
	DelWindowFailed();
};

class BoxFailed: public BaseCurEx {
    public:
	BoxFailed();
};

class DoupdateFailed: public BaseCurEx {
    public:
	DoupdateFailed();
};

class AddStrFailed: public BaseCurEx {
    public:
	AddStrFailed();
};

class EraseFailed: public BaseCurEx {
    public:
	EraseFailed();
};

class ScrollOKFailed: public BaseCurEx {
    public:
	ScrollOKFailed();
};

class LeaveOKFailed: public BaseCurEx {
    public:
	LeaveOKFailed();
};

class ClearOKFailed: public BaseCurEx {
    public:
	ClearOKFailed();
};

class NoNLFailed: public BaseCurEx {
    public:
	NoNLFailed();
};

class AlreadyRealized: public BaseCurEx {
    public:
	AlreadyRealized();
};

class NotRealized: public BaseCurEx {
    public:
	NotRealized();
};

class SystemError: public BaseCurEx {
    public:
	SystemError(int _errno);
};

class WinSizeInvalid: public BaseCurEx {
    public:
	WinSizeInvalid();
};

class UnableToGetWinSize: public BaseCurEx {
    public:
	UnableToGetWinSize();
};

class DelCurTermFailed: public BaseCurEx {
    public:
	DelCurTermFailed();
};

class SetupTermFailed: public BaseCurEx {
    public:
	SetupTermFailed();
};

class TouchFailed: public BaseCurEx {
    public:
	TouchFailed();
};

class CbreakFailed: public BaseCurEx {
    public:
	CbreakFailed();
};

class NoEchoFailed: public BaseCurEx {
    public:
	NoEchoFailed();
};

class SubwinFailed: public BaseCurEx {
    public:
	SubwinFailed();
};

class ClearFailed: public BaseCurEx {
    public:
	ClearFailed();
};

class CannotFocus: public BaseCurEx {
    public:
	CannotFocus();
};

class KeyPadFailed: public BaseCurEx {
    public:
	KeyPadFailed();
};

class UnexpectedEvent: public BaseCurEx {
    public:
	UnexpectedEvent();
};

class UnableToStartColor: public BaseCurEx {
    public:
	UnableToStartColor();
};

class ColorsNotInitialized: public BaseCurEx {
    public:
	ColorsNotInitialized();
};

class WMoveFailed: public BaseCurEx {
    public:
	WMoveFailed();
};

class AreaExceeded: public BaseCurEx {
    public:
	AreaExceeded();
};

class WInsChFailed: public BaseCurEx {
    public:
	WInsChFailed();
};

#endif // CUREX_H
