#!/bin/sh

for h in fish dash debian32 freebsd32 openbsd32 netbsd32 abraxas aurora starchild odin wheezy puffy hyperion
do
    TESTDIR="${HOME}/libyacurs-test"
    TESTDIRHOST="${TESTDIR}/${h}"

    if [ ! -d "${TESTDIR}" ]
    then
	mkdir "${TESTDIR}"
    fi

    if [ -d "${TESTDIRHOST}" ]
    then
	cd "${TESTDIRHOST}"
	svn update || exit 1
    else
	svn co https://gizmo.kruemel.home/svn/swprojects/libyacurs/trunk "${TESTDIRHOST}" || exit 1
    fi
    
    cd "${TESTDIRHOST}" || exit 1
    touch ChangeLog
    touch README
    autoreconf -fi || exit 1

    xterm -T "${h}" -n "${h}" -e "ssh -t $h 'cd ${TESTDIRHOST} ; if [ -x /usr/xpg4/bin/sh ] ; then /usr/xpg4/bin/sh checkall.sh ; else /bin/sh scripts/checkall.sh ; fi ; echo '============= DONE =============' ; read BLA'" &
done

exit 0
