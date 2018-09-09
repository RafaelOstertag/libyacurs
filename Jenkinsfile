pipeline {
    agent any

    options {
        ansiColor('xterm')
        buildDiscarder logRotator(artifactDaysToKeepStr: '', artifactNumToKeepStr: '', daysToKeepStr: '', numToKeepStr: '10')
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
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

                        stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                            }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
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
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

						stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
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
                                    sh "../configure --enable-debug --disable-silent-rules CC=cc CXX=c++ CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

						stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
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
                                    sh "../configure --enable-debug --disable-silent-rules CXXFLAGS='-Wall -pedantic'"
                                }
                            }
                        }

						stage("Build") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE all'
                                }
                             }
                        }

                        stage("Test") {
                            steps {
                                dir("obj") {
                                    sh '$MAKE check'
                                }
                            }
                        }
					}
				} // stage("NetBSD")
    		} // parallel
        } // stage("OS Build")
    } // stages
}
