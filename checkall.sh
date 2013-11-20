#!/bin/sh
#
# $Id$

TERM=xterm
export TERM

had_error() {
	retval=$1
	msg=$2

	if [ $retval -ne 0 ]
	then
	    echo "$msg"
	    exit $retval
	fi
}

DASH_CXXFLAGS_43='-march=native -mtune=native -O3 -pedantic -Wall -Werror'
DASH_CFLAGS_43="$DASH_CXXFLAGS_43"
DASH_CC_43=gcc-4.3
DASH_CXX_43=g++-4.3
DASH_LDFLAGS_43="-L/usr/lib"

DASH_CXXFLAGS_44="$DASH_CXXFLAGS_43"
DASH_CFLAGS_44="$DASH_CXXFLAGS_43"
DASH_CC_44=gcc-4.4
DASH_CXX_44=g++-4.4
DASH_LDFLAGS_44="-L/usr/lib"

DASH_CXXFLAGS_default="$DASH_CXXFLAGS_43"
DASH_CFLAGS_default="$DASH_CXXFLAGS_43"
DASH_CC_default=gcc
DASH_CXX_default=g++
DASH_LDFLAGS_default="-L/usr/lib"

DASH_CXXFLAGS_clang="$DASH_CXXFLAGS_43"
DASH_CFLAGS_clang="$DASH_CXXFLAGS_43"
DASH_CC_clang=clang
DASH_CXX_clang=clang++
DASH_LDFLAGS_clang="-L/usr/lib"

host_dash() {
    for c in default clang 43 44
    do
	make distclean
	./configure CXX="`eval echo \\$DASH_CXX_$c`" \
	    CC="`eval echo \\$DASH_CC_$c`" \
	    CFLAGS="`eval echo \\$DASH_CFLAGS_$c`" \
	    CXXFLAGS="`eval echo \\$DASH_CXXFLAGS_$c`" \
	    LDFLAGS="`eval echo \\$DASH_LDFLAGS_$c`"

	had_error $? "Error in CXX=`eval echo \\$DASH_CXX_$c` CC=`eval echo \\$DASH_CC_$c` CFLAGS=`eval echo \\$DASH_CFLAGS_$c` CXXFLAGS=`eval echo \\$DASH_CXXFLAGS_$c` LDFLAGS=`eval echo \\$DASH_LDFLAGS_$c`"

	make clean
	make
	had_error $? "Error in CXX=`eval echo \\$DASH_CXX_$c` CC=`eval echo \\$DASH_CC_$c` CFLAGS=`eval echo \\$DASH_CFLAGS_$c` CXXFLAGS=`eval echo \\$DASH_CXXFLAGS_$c` LDFLAGS=`eval echo \\$DASH_LDFLAGS_$c`"

	make check
	had_error $? "Error in CXX=`eval echo \\$DASH_CXX_$c` CC=`eval echo \\$DASH_CC_$c` CFLAGS=`eval echo \\$DASH_CFLAGS_$c` CXXFLAGS=`eval echo \\$DASH_CXXFLAGS_$c` LDFLAGS=`eval echo \\$DASH_LDFLAGS_$c`"
    done
}

WHEEZY_CXXFLAGS_44='-march=native -mtune=native -O3 -pedantic -Wall -Werror'
WHEEZY_CFLAGS_44="$WHEEZY_CXXFLAGS_44"
WHEEZY_CC_44=gcc-4.4
WHEEZY_CXX_44=g++-4.4
WHEEZY_LDFLAGS_44="-L/usr/lib"

WHEEZY_CXXFLAGS_46="$WHEEZY_CXXFLAGS_44"
WHEEZY_CFLAGS_46="$WHEEZY_CXXFLAGS_44"
WHEEZY_CC_46=gcc-4.6
WHEEZY_CXX_46=g++-4.6
WHEEZY_LDFLAGS_46="-L/usr/lib"

WHEEZY_CXXFLAGS_47="$WHEEZY_CXXFLAGS_44"
WHEEZY_CFLAGS_47="$WHEEZY_CXXFLAGS_44"
WHEEZY_CC_47=gcc-4.7
WHEEZY_CXX_47=g++-4.7
WHEEZY_LDFLAGS_47="-L/usr/lib"

WHEEZY_CXXFLAGS_default="$WHEEZY_CXXFLAGS_44"
WHEEZY_CFLAGS_default="$WHEEZY_CXXFLAGS_44"
WHEEZY_CC_default=gcc
WHEEZY_CXX_default=g++
WHEEZY_LDFLAGS_default="-L/usr/lib"

WHEEZY_CXXFLAGS_clang="$WHEEZY_CXXFLAGS_44"
WHEEZY_CFLAGS_clang="$WHEEZY_CXXFLAGS_44"
WHEEZY_CC_clang=clang
WHEEZY_CXX_clang=clang++
WHEEZY_LDFLAGS_clang="-L/usr/lib"

host_wheezy() {
    for c in default clang 44 46 47
    do
	make distclean
	./configure CXX="`eval echo \\$WHEEZY_CXX_$c`" \
	    CC="`eval echo \\$WHEEZY_CC_$c`" \
	    CFLAGS="`eval echo \\$WHEEZY_CFLAGS_$c`" \
	    CXXFLAGS="`eval echo \\$WHEEZY_CXXFLAGS_$c`" \
	    LDFLAGS="`eval echo \\$WHEEZY_LDFLAGS_$c`"

	had_error $? "Error in CXX=`eval echo \\$WHEEZY_CXX_$c` CC=`eval echo \\$WHEEZY_CC_$c` CFLAGS=`eval echo \\$WHEEZY_CFLAGS_$c` CXXFLAGS=`eval echo \\$WHEEZY_CXXFLAGS_$c` LDFLAGS=`eval echo \\$WHEEZY_LDFLAGS_$c`"

	make clean
	make
	had_error $? "Error in CXX=`eval echo \\$WHEEZY_CXX_$c` CC=`eval echo \\$WHEEZY_CC_$c` CFLAGS=`eval echo \\$WHEEZY_CFLAGS_$c` CXXFLAGS=`eval echo \\$WHEEZY_CXXFLAGS_$c` LDFLAGS=`eval echo \\$WHEEZY_LDFLAGS_$c`"

	make check
	had_error $? "Error in CXX=`eval echo \\$WHEEZY_CXX_$c` CC=`eval echo \\$WHEEZY_CC_$c` CFLAGS=`eval echo \\$WHEEZY_CFLAGS_$c` CXXFLAGS=`eval echo \\$WHEEZY_CXXFLAGS_$c` LDFLAGS=`eval echo \\$WHEEZY_LDFLAGS_$c`"
    done
}

DEBIAN32_CXXFLAGS_43='-march=native -mtune=native -O3 -pedantic -Wall -Werror'
DEBIAN32_CFLAGS_43="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CC_43=gcc-4.3
DEBIAN32_CXX_43=g++-4.3
DEBIAN32_LDFLAGS_43="-L/usr/lib"

DEBIAN32_CXXFLAGS_44="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CFLAGS_44="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CC_44=gcc-4.4
DEBIAN32_CXX_44=g++-4.4
DEBIAN32_LDFLAGS_44="-L/usr/lib"

DEBIAN32_CXXFLAGS_default="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CFLAGS_default="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CC_default=gcc
DEBIAN32_CXX_default=g++
DEBIAN32_LDFLAGS_default="-L/usr/lib"

DEBIAN32_CXXFLAGS_clang="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CFLAGS_clang="$DEBIAN32_CXXFLAGS_43"
DEBIAN32_CC_clang=clang
DEBIAN32_CXX_clang=clang++
DEBIAN32_LDFLAGS_clang="-L/usr/lib"

host_debian32() {
    # clang has been removed from list, it currently fails when running the tests
    for c in default 43 44
    do
	make distclean
	./configure CXX="`eval echo \\$DEBIAN32_CXX_$c`" \
	    CC="`eval echo \\$DEBIAN32_CC_$c`" \
	    CFLAGS="`eval echo \\$DEBIAN32_CFLAGS_$c`" \
	    CXXFLAGS="`eval echo \\$DEBIAN32_CXXFLAGS_$c`" \
	    LDFLAGS="`eval echo \\$DEBIAN32_LDFLAGS_$c`"

	had_error $? "Error in CXX=`eval echo \\$DEBIAN32_CXX_$c` CC=`eval echo \\$DEBIAN32_CC_$c` CFLAGS=`eval echo \\$DEBIAN32_CFLAGS_$c` CXXFLAGS=`eval echo \\$DEBIAN32_CXXFLAGS_$c` LDFLAGS=`eval echo \\$DEBIAN32_LDFLAGS_$c`"

	make clean
	make
	had_error $? "Error in CXX=`eval echo \\$DEBIAN32_CXX_$c` CC=`eval echo \\$DEBIAN32_CC_$c` CFLAGS=`eval echo \\$DEBIAN32_CFLAGS_$c` CXXFLAGS=`eval echo \\$DEBIAN32_CXXFLAGS_$c` LDFLAGS=`eval echo \\$DEBIAN32_LDFLAGS_$c`"

	make check
	had_error $? "Error in CXX=`eval echo \\$DEBIAN32_CXX_$c` CC=`eval echo \\$DEBIAN32_CC_$c` CFLAGS=`eval echo \\$DEBIAN32_CFLAGS_$c` CXXFLAGS=`eval echo \\$DEBIAN32_CXXFLAGS_$c` LDFLAGS=`eval echo \\$DEBIAN32_LDFLAGS_$c`"
    done
}

AURORA_CXXFLAGS_42='-march=native -mtune=native -O3 -pedantic -Wall -Werror'
AURORA_CFLAGS_42="$AURORA_CXXFLAGS_42"
AURORA_CC_42=gcc42
AURORA_CXX_42=g++42
AURORA_LDFLAGS_42="-L/usr/local/lib/gcc42 -R/usr/local/lib/gcc42"

AURORA_CXXFLAGS_44="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_44="$AURORA_CXXFLAGS_42"
AURORA_CC_44=gcc44
AURORA_CXX_44=g++44
AURORA_LDFLAGS_44="-L/usr/local/lib/gcc44 -R/usr/local/lib/gcc44"

AURORA_CXXFLAGS_46="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_46="$AURORA_CXXFLAGS_42"
AURORA_CC_46=gcc46
AURORA_CXX_46=g++46
AURORA_LDFLAGS_46="-L/usr/local/lib/gcc46 -R/usr/local/lib/gcc46"

AURORA_CXXFLAGS_47="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_47="$AURORA_CXXFLAGS_42"
AURORA_CC_47=gcc47
AURORA_CXX_47=g++47
AURORA_LDFLAGS_47="-L/usr/local/lib/gcc47 -R/usr/local/lib/gcc47"

AURORA_CXXFLAGS_48="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_48="$AURORA_CXXFLAGS_42"
AURORA_CC_48=gcc48
AURORA_CXX_48=g++48
AURORA_LDFLAGS_48="-L/usr/local/lib/gcc48 -R/usr/local/lib/gcc48"

AURORA_CXXFLAGS_default="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_default="$AURORA_CXXFLAGS_42"
AURORA_CC_default=gcc
AURORA_CXX_default=g++
AURORA_LDFLAGS_default=""

AURORA_CXXFLAGS_clang="$AURORA_CXXFLAGS_42"
AURORA_CFLAGS_clang="$AURORA_CXXFLAGS_42"
AURORA_CC_clang=clang
AURORA_CXX_clang=clang++
AURORA_LDFLAGS_clang=""

AURORA_CURSES_CPPFLAGS_system=
AURORA_CURSES_LDFLAGS_system=

AURORA_CURSES_CPPFLAGS_ncurses='-I/usr/local/include'
AURORA_CURSES_LDFLAGS_ncurses='-L/usr/local/lib'

host_aurora() {
    for c in default clang 42 44 46 47 48
    do
	for curs in system
	do
	    gmake distclean
	    ./configure CXX="`eval echo \\$AURORA_CXX_$c`" \
		CC="`eval echo \\$AURORA_CC_$c`" \
		CFLAGS="`eval echo \\$AURORA_CFLAGS_$c`" \
		CPPFLAGS="`eval echo \\$AURORA_CURSES_CPPFLAGS_$curs`" \
		CXXFLAGS="`eval echo \\$AURORA_CXXFLAGS_$c`" \
		LDFLAGS="`eval echo \\$AURORA_LDFLAGS_$c` `eval echo \\$AURORA_CURSES_LDFLAGS_$curs`"

	    had_error $? "Error in CXX="`eval echo \\$AURORA_CXX_$c`" CC="`eval echo \\$AURORA_CC_$c`" CFLAGS="`eval echo \\$AURORA_CFLAGS_$c`" CPPFLAGS="`eval echo \\$AURORA_CURSES_CPPFLAGS_$curs`" 	CXXFLAGS="`eval echo \\$AURORA_CXXFLAGS_$c`" LDFLAGS="`eval echo \\$AURORA_LDFLAGS_$c` `eval echo \\$AURORA_CURSES_LDFLAGS_$curs`""

	    gmake clean
	    gmake
	    had_error $? "Error in CXX="`eval echo \\$AURORA_CXX_$c`" CC="`eval echo \\$AURORA_CC_$c`" CFLAGS="`eval echo \\$AURORA_CFLAGS_$c`" CPPFLAGS="`eval echo \\$AURORA_CURSES_CPPFLAGS_$curs`" 	CXXFLAGS="`eval echo \\$AURORA_CXXFLAGS_$c`" LDFLAGS="`eval echo \\$AURORA_LDFLAGS_$c` `eval echo \\$AURORA_CURSES_LDFLAGS_$curs`""
	    
	    gmake check
	    had_error $? "Error in CXX="`eval echo \\$AURORA_CXX_$c`" CC="`eval echo \\$AURORA_CC_$c`" CFLAGS="`eval echo \\$AURORA_CFLAGS_$c`" CPPFLAGS="`eval echo \\$AURORA_CURSES_CPPFLAGS_$curs`" 	CXXFLAGS="`eval echo \\$AURORA_CXXFLAGS_$c`" LDFLAGS="`eval echo \\$AURORA_LDFLAGS_$c` `eval echo \\$AURORA_CURSES_LDFLAGS_$curs`""
	done
    done
}

FREEBSD32_CXXFLAGS_42='-march=native -mtune=native -O3 -pedantic -Wall -Werror'
FREEBSD32_CFLAGS_42="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_42=gcc42
FREEBSD32_CXX_42=g++42
FREEBSD32_LDFLAGS_42="-L/usr/local/lib/gcc42 -R/usr/local/lib/gcc42"

FREEBSD32_CXXFLAGS_44="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CFLAGS_44="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_44=gcc44
FREEBSD32_CXX_44=g++44
FREEBSD32_LDFLAGS_44="-L/usr/local/lib/gcc44 -R/usr/local/lib/gcc44"

FREEBSD32_CXXFLAGS_46="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CFLAGS_46="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_46=gcc46
FREEBSD32_CXX_46=g++46
FREEBSD32_LDFLAGS_46="-L/usr/local/lib/gcc46 -R/usr/local/lib/gcc46"

FREEBSD32_CXXFLAGS_47="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CFLAGS_47="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_47=gcc47
FREEBSD32_CXX_47=g++47
FREEBSD32_LDFLAGS_47="-L/usr/local/lib/gcc47 -R/usr/local/lib/gcc47"

FREEBSD32_CXXFLAGS_48="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CFLAGS_48="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_48=gcc48
FREEBSD32_CXX_48=g++48
FREEBSD32_LDFLAGS_48="-L/usr/local/lib/gcc48 -R/usr/local/lib/gcc48"

FREEBSD32_CXXFLAGS_default="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CFLAGS_default="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_default=gcc
FREEBSD32_CXX_default=g++
FREEBSD32_LDFLAGS_default=""

FREEBSD32_CXXFLAGS_clang="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CFLAGS_clang="$FREEBSD32_CXXFLAGS_42"
FREEBSD32_CC_clang=clang
FREEBSD32_CXX_clang=clang++
FREEBSD32_LDFLAGS_clang=""

FREEBSD32_CURSES_CPPFLAGS_system=
FREEBSD32_CURSES_LDFLAGS_system=

FREEBSD32_CURSES_CPPFLAGS_ncurses='-I/usr/local/include'
FREEBSD32_CURSES_LDFLAGS_ncurses='-L/usr/local/lib'

host_freebsd32() {
    for c in default clang 42 44 46 47 48
    do
	for curs in system
	do
	    gmake distclean
	    ./configure --disable-nls CXX="`eval echo \\$FREEBSD32_CXX_$c`" \
		CC="`eval echo \\$FREEBSD32_CC_$c`" \
		CFLAGS="`eval echo \\$FREEBSD32_CFLAGS_$c`" \
		CPPFLAGS="`eval echo \\$FREEBSD32_CURSES_CPPFLAGS_$curs`" \
		CXXFLAGS="`eval echo \\$FREEBSD32_CXXFLAGS_$c`" \
		LDFLAGS="`eval echo \\$FREEBSD32_LDFLAGS_$c` `eval echo \\$FREEBSD32_CURSES_LDFLAGS_$curs`"

	    had_error $? "Error in CXX="`eval echo \\$FREEBSD32_CXX_$c`" CC="`eval echo \\$FREEBSD32_CC_$c`" CFLAGS="`eval echo \\$FREEBSD32_CFLAGS_$c`" CPPFLAGS="`eval echo \\$FREEBSD32_CURSES_CPPFLAGS_$curs`" 	CXXFLAGS="`eval echo \\$FREEBSD32_CXXFLAGS_$c`" LDFLAGS="`eval echo \\$FREEBSD32_LDFLAGS_$c` `eval echo \\$FREEBSD32_CURSES_LDFLAGS_$curs`""

	    gmake clean
	    gmake
	    had_error $? "Error in CXX="`eval echo \\$FREEBSD32_CXX_$c`" CC="`eval echo \\$FREEBSD32_CC_$c`" CFLAGS="`eval echo \\$FREEBSD32_CFLAGS_$c`" CPPFLAGS="`eval echo \\$FREEBSD32_CURSES_CPPFLAGS_$curs`" 	CXXFLAGS="`eval echo \\$FREEBSD32_CXXFLAGS_$c`" LDFLAGS="`eval echo \\$FREEBSD32_LDFLAGS_$c` `eval echo \\$FREEBSD32_CURSES_LDFLAGS_$curs`""
	    
	    gmake check
	    had_error $? "Error in CXX="`eval echo \\$FREEBSD32_CXX_$c`" CC="`eval echo \\$FREEBSD32_CC_$c`" CFLAGS="`eval echo \\$FREEBSD32_CFLAGS_$c`" CPPFLAGS="`eval echo \\$FREEBSD32_CURSES_CPPFLAGS_$curs`" 	CXXFLAGS="`eval echo \\$FREEBSD32_CXXFLAGS_$c`" LDFLAGS="`eval echo \\$FREEBSD32_LDFLAGS_$c` `eval echo \\$FREEBSD32_CURSES_LDFLAGS_$curs`""
	done
    done
}

host_abraxas() {
    gmake distclean
    ./configure \
	CFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native" \
	CXXFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native"
    had_error $? "Error on abraxas"

    gmake clean
    gmake
    had_error $? "Error on abraxas"

    gmake check
    had_error $? "Error on abraxas"
}

host_netbsd32() {
    gmake distclean
    ./configure \
	CFLAGS="-O3 -Wall -Werror -pedantic" \
	CXXFLAGS="-O3 -Wall -Werror -pedantic"
    had_error $? "Error on netbsd32"

    gmake clean
    gmake
    had_error $? "Error on netbsd32"

    gmake check
    had_error $? "Error on netbsd32"
}

host_fish() {
    gmake distclean
    ./configure \
	CFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native" \
	CXXFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native"
    had_error $? "Error on fish"

    gmake clean
    gmake
    had_error $? "Error on fish"

    gmake check
    had_error $? "Error on fish"
}

host_puffy() {
    gmake distclean
    ./configure \
	CFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native" \
	CXXFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native"
    had_error $? "Error on fish"

    gmake clean
    gmake
    had_error $? "Error on fish"

    gmake check
    had_error $? "Error on fish"
}

host_openbsd32() {
    gmake distclean
    ./configure \
	CFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native" \
	CXXFLAGS="-O3 -Wall -Werror -pedantic -march=native -mtune=native"
    had_error $? "Error in OpenBSD32"

    gmake clean
    gmake
    had_error $? "Error in OpenBSD32"

    gmake check
    had_error $? "Error in OpenBSD32"
}

host_odin() {
    for t in xterm xterm-color
    do
	export TERM=$t
	for c in /opt/solarisstudio12.3/bin /opt/solstudio12.2/bin /opt/sunstudio12.1/bin
	do
	    for arch in -m64 -m32
	    do
		for flags in xpg4 solaris ncurses
		do
		    case $flags in
			xpg4)
			    LDFLAGS="-L/usr/xpg4/lib/${arch#-m} -R/usr/xpg4/lib/${arch#-m}"
			    CPPFLAGS="-I/usr/xpg4/include"
			    CONFIGARGS="--without-ncurses"
			    ;;
			solaris)
			    CONFIGARGS="--without-ncurses"
			    ;;
			ncurses)
			    CPPFLAGS="-I/usr/include/ncurses"
			    CONFIGARGS="--with-ncurses"
			    ;;
		    esac

		    gmake distclean
		    ./configure CXX=${c}/CC CC=${c}/cc LDFLAGS="$LDFLAGS" CPPFLAGS="$CPPFLAGS" CFLAGS="-fast $arch" CXXFLAGS="-fast $arch" $CONFIGARGS
		    had_error $? "Error in ${c}:$arch:$flags"

		    gmake
		    had_error $? "Error in ${c}:$arch:$flags"

		    gmake check
		    had_error $? "Error in ${c}:$arch:$flags"
		done
	    done
	done
    done
}

host_starchild() {
    PATH=/usr/ccs/bin:/usr/sfw/bin:$PATH
    export PATH
    for t in xterm xtermc
    do
	export TERM=$t
	for c in /opt/solarisstudio12.3/bin /opt/solstudio12.2/bin /opt/sunstudio12.1/bin
	do
	    for arch in -m64 -m32
	    do
		for flags in xpg4 solaris
		do
		    unset LDFLAGS CPPFLAGS
		    case $flags in
			xpg4)
			    case $arch in
				-m64)
				    LDFLAGS="-L/usr/xpg4/lib/${arch#-m} -R/usr/xpg4/lib/${arch#-m}"
				    CPPFLAGS="-I/usr/xpg4/include"
				    ;;
				-m32)
				    LDFLAGS="-L/usr/xpg4/lib -R/usr/xpg4/lib"
				    CPPFLAGS="-I/usr/xpg4/include"
				    ;;
			    esac
			    ;;
			solaris)
			    :
			    ;;
			ncurses)
			    CPPFLAGS="-I/usr/include/ncurses"
			    ;;
		    esac

		    export LDFLAGS
		    export CPPFLAGS

		    gmake distclean
		    ./configure CXX=${c}/CC CC=${c}/cc CFLAGS="-fast $arch" CXXFLAGS="-fast $arch"
		    had_error $? "Error in ${c}:$arch:$flags"

		    gmake -j4
		    had_error $? "Error in ${c}:$arch:$flags"

		    gmake check
		    had_error $? "Error in ${c}:$arch:$flags"
		done
	    done
	done
    done
}



HOST="`uname -n | sed 's/\..*//'`"

host_$HOST
