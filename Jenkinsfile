node("freebsd") {
    stage("checkout") {
	checkout scm
    }

    stage("autoconf") {
	sh "touch ChangeLog"
	sh "autoreconf -I m4 -i"
    }

    stage("configure") {
	sh "./configure"
    }

    stage("build") {
	sh "gmake all"
    }

    stage("check") {
	sh "gmake tests"
    }
}
