/*
 * Used to replace curses' wgetch(). It will call __test_wgetch()
 * which you can define in your app to simulate wgetch().
 *
 * Use the library with LD_PRELOAD, e.g.
 *
 *  $ LD_PRELOAD=./libpreload.so app1
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(HAVE_CURSES_ENHANCED) && \
    defined(HAVE_LOCALE_H) && \
    defined(HAVE_CWCHAR)
# define USE_WCHAR 1
# include <locale.h>
# include <cwchar>
#else
# undef USE_WCHAR
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USE_WCHAR
int __test_wget_wch(void*, std::wint_t* i);

int
wget_wch(void* wdc, std::wint_t* i) {
    return __test_wget_wch(wdc, i);
}

#else
// Forward declaration. Define those functions in your executable.
int __test_wgetch(void*);

int
wgetch(void* wdc) {
    return __test_wgetch(wdc);
}

#endif

#ifdef __cplusplus
}
#endif
