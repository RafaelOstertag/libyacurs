/*
 * Used to replace curses' wgetch(). It will call __test_wgetch()
 * which you can define in your app to simulate wgetch().
 *
 * Use the library with LD_PRELOAD, e.g.
 *
 *  $ LD_PRELOAD=./libpreload.so app1
 *
 */

#ifdef __cplusplus
extern "C" {
#endif
    int __test_wgetch(void*);
    
    int wgetch(void* wdc) {
	return __test_wgetch(wdc);
    }

#ifdef __cplusplus
};
#endif
