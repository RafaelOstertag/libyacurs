pipeline {
    agent any

    options {
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
        timeout(time: 1, unit: 'HOURS')
        timestamps()
    }

    environment {
        PEDANTIC_FLAGS = "-Wall -pedantic -Werror -O3 -Wno-unknown-pragmas"
    }

    stages {
        stage("OS Build") {
            parallel {
                stage("FreeBSD") {
                    agent {
                        label "freebsd"
                    }
                    stages {
                        stage("Bootstrap Build") {
                             steps {
                                sh "git log --stat > ChangeLog"
                                dir("libyacurs") {
                                    sh "git log --stat > ChangeLog"
                                }
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules"
                                }
                            }
                        }

                        stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
                    }
                } // stage("FreeBSD")

				stage("Linux") {
					agent {
						label "linux"
					}
					stages {
						stage("Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules"
                                }
                            }
                        }

						stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("Linux")

				stage("OpenBSD") {
					agent {
						label "openbsd"
					}
					stages {
						stage("Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules CC=cc CXX=c++"
                                }
                            }
                        }

						stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("OpenBSD")

				stage("NetBSD") {
					agent {
						label "netbsd"
					}
					stages {
						stage("Bootstrap Build") {
                             steps {
                                sh "touch ChangeLog"
                                dir("libyacurs") {
                                    sh "touch ChangeLog"
                                }
                                sh "autoreconf -I m4 -i"
                            }
                        }

                        stage("Configure") {
                            steps {
                                dir("obj") {
                                    sh "../configure --enable-debug --disable-silent-rules"
                                }
                            }
                        }

						stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                             }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check CXXFLAGS="${PEDANTIC_FLAGS}"'
                                }
                            }
                        }
					}
				} // stage("NetBSD")
    		} // parallel
        } // stage("OS Build")
    } // stages
}
