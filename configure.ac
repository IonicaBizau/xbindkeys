dnl Process this file with autoconf to produce a configure script.
AC_INIT(xbindkeys, 1.8.6, hocwp@free.fr)
AC_PREREQ(2.54)
AC_COPYRIGHT(Copyright (C) Philippe Brochard <hocwp@free.fr>)

# Automake
AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION, $PACKAGE_BUGREPORT)
AM_MAINTAINER_MODE

AC_CONFIG_SRCDIR([xbindkeys.c])
AM_CONFIG_HEADER(config.h)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_AWK

dnl Checks for libraries.
dnl there are none here...see the X libraries section below!

dnl Checks for header files.
AC_PATH_X
AC_PATH_XTRA
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

dnl Checks for X libraries.
LIBS_saved="$LIBS"
LIBS=
AC_CHECK_LIB(X11, XCreateWindow,, [
	AC_MSG_WARN([Xbindkeys depends on the X11 libraries!])
], $LIBS_saved $X_LIBS)
X_LIBS="$X_LIBS $LIBS"
LIBS="$LIBS_saved"


dnl disable fork+exec start
use_fork=yes

AC_ARG_ENABLE( fork,
[  --disable-fork       No fork+exec to launch program (Use a system call instead)],
   use_fork="${enableval}")

FORK_FLAG=""
if test "$use_fork" = yes; then
   FORK_FLAG="-DFORK_FLAG=1"
fi

AC_SUBST(FORK_FLAG)


dnl disable xbindkeys_show install
use_tk=yes

AC_ARG_ENABLE(tk,
[  --disable-tk    Do not install xbindkeys_show (prevent tk dependency)],
   use_tk="${enableval}")

AM_CONDITIONAL(TK, test x$use_tk = xyes)


dnl disable guile config file
use_guile=yes

AC_ARG_ENABLE( guile,
[  --disable-guile       Disable scheme/guile configuration file style ],
   use_guile="${enableval}")

GUILE_FLAG=""

if test "$use_guile" = yes; then
   GUILE_FLAGS
   GUILE_FLAG="-DGUILE_FLAG=1"
fi

AM_CONDITIONAL(GUILE, test x$use_guile = xyes)

AC_SUBST(GUILE_FLAG)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T


# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([strcasecmp strchr strrchr strstr strtol])

dnl final output
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
