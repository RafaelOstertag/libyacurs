// $Id$
//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <iostream>
#include <cstdlib>

#include "yacurs.h"

// Used when preloading libtestpreload.so

#ifdef USE_WCHAR
wint_t __test_str[] = { 
    L'H', L'e', L'l', L'l', L'o', L',', L' ', L'W', L'o', L'r', L'l', L'd', L'!', L'\0'
};

extern "C" int
__test_wget_wch(void*, wint_t* i) {
    static wint_t* ptr = __test_str;

    *i=*ptr++;
    return OK;
}
#else
char __test_str[] = "Hello, World!";

extern "C" int
__test_wgetch(void*) {
    static char* ptr = __test_str;

    return *ptr++;
}
#endif

class Handler {
    private:
        YACURS::EVENT_TYPE expected_evt;
        int calls;
    public:
        Handler(YACURS::EVENT_TYPE evt) :
            expected_evt(evt), calls(0) {
        }

        Handler(const Handler& _h) {
            expected_evt = _h.expected_evt;
            calls = _h.calls;
        }

        virtual ~Handler() {
        }

        virtual void handler(YACURS::Event& e) {
            if (e.type() != expected_evt) {
                std::cerr << "Unexpected event type" << std::endl;
                std::abort();
            }

            calls++;
        }

        int getCalls() {
            return calls;
        }

        void reset() {
            calls = 0;
        }
};

class MyHandler : public Handler {
    private:
        std::string _str;

    public:
        MyHandler() : Handler(YACURS::EVT_KEY), _str() {
        }

        void handler(YACURS::Event& e) {
            Handler::handler(e);

#ifdef USE_WCHAR
	    if (typeid (e) != typeid (YACURS::EventEx<wint_t>) ) std::abort();
	    YACURS::EventEx<wint_t>& tmp =
		dynamic_cast<YACURS::EventEx<wint_t>&>(e);

	    char *tmp_str = new char[MB_CUR_MAX];
	    int retval=wctomb(tmp_str, tmp.data());
	    if (retval==-1) {
		delete[] tmp_str;
		std::cerr << "Unable to convert wchar to MB char" << std::endl;
		abort();
	    }
	    tmp_str[retval]='\0';

	    _str+=tmp_str;

	    delete[] tmp_str;
#else
            if (typeid (e) != typeid (YACURS::EventEx<int>) ) std::abort();
            YACURS::EventEx<int>& tmp =
                dynamic_cast<YACURS::EventEx<int>&>(e);

            char c = static_cast<char>(tmp.data() );
            _str.push_back(c);
#endif

            if (_str == "Hello, World!")
                YACURS::EventQueue::submit(YACURS::Event(YACURS::EVT_QUIT) );
        }
};

int
main() {
#ifdef USE_WCHAR
    setlocale(LC_ALL, "");
#endif

    try {
        MyHandler h;
        YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<
                                              MyHandler>(YACURS::EVT_KEY, &h,
                                                         &MyHandler::handler) );

        YACURS::Curses::init();

        YACURS::EventQueue::run();

        YACURS::Curses::end();
    } catch (std::exception& e) {
        YACURS::Curses::end();
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
