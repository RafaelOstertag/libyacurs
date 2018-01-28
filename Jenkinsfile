node {
    stage("checkout") {
	checkout scm
    }

    stage("autoconf") {
	sh "touch ChangeLog"
	sh "autoreconf -I m4 -i"
    }
}
