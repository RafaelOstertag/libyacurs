#!/bin/sh

if [ -d ~/libyacurs-test ]
then
    cd ~/libyacurs-test
    svn update || exit 1
else
    svn co svn+ssh://gizmo.kruemel.home/var/svn/swprojects/libyacurs/trunk ~/libyacurs-test || exit 1
fi

cd ~/libyacurs-test || exit 1
make -f Makefile.boot || exit 1

for h in fish dash debian32 freebsd32 openbsd32 netbsd32 abraxas aurora starchild odin
do
    ssh -t $h "cd $HOME/libyacurs-test ; if [ -x /usr/xpg4/bin/sh ] ; then /usr/xpg4/bin/sh checkall.sh ; else /bin/sh checkall.sh ; fi"
    if [ $? -ne 0 ]
    then
	echo "ERROR on $h"
	exit 1
    fi
done

exit 0
