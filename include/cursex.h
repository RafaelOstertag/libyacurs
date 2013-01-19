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
 * @defgroup exceptions Exceptions.
 *
 * @{
 */

/**
 * Base class used for all exception in yacurs.
 *
 */
class BaseCurEx: public std::exception {
    private:
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

class DerWinFailed: public BaseCurEx {
    public:
	DerWinFailed();
};

class ClearFailed: public BaseCurEx {
    public:
	ClearFailed();
};

/**
 * @}
 */

#endif // CUREX_H
