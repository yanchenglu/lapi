dnl $Id$
dnl config.m4 for extension lapi

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(lapi, for lapi support,
dnl Make sure that the comment is aligned:
dnl [  --with-lapi             Include lapi support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(lapi, whether to enable lapi support,
dnl Make sure that the comment is aligned:
[  --enable-lapi           Enable lapi support])

if test "$PHP_LAPI" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-lapi -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/lapi.h"  # you most likely want to change this
  dnl if test -r $PHP_LAPI/$SEARCH_FOR; then # path given as parameter
  dnl   LAPI_DIR=$PHP_LAPI
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for lapi files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       LAPI_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$LAPI_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the lapi distribution])
  dnl fi

  dnl # --with-lapi -> add include path
  dnl PHP_ADD_INCLUDE($LAPI_DIR/include)

  dnl # --with-lapi -> check for lib and symbol presence
  dnl LIBNAME=lapi # you may want to change this
  dnl LIBSYMBOL=lapi # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $LAPI_DIR/$PHP_LIBDIR, LAPI_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_LAPILIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong lapi lib version or lib not found])
  dnl ],[
  dnl   -L$LAPI_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(LAPI_SHARED_LIBADD)

  PHP_NEW_EXTENSION(lapi, lapi.c lapi_application.c lapi_load.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi

if test -z "$PHP_DEBUG"; then
    AC_ARG_ENABLE(debug,
    [ --enable-debug      compile with debugging symbols],[
    PHP_DEBUG=$enableval
    ],[ PHP_DEBUG=no
    ])
fi
