AC_INIT([madstore], [0.1])

AM_INIT_AUTOMAKE
AC_PROG_RANLIB
AC_LANG([C++])
AC_PROG_CXX
AC_SUBST([CC])

[CPPFLAGS="${CPPFLAGS} -std=c++11"]

[CPPFLAGS="${CPPFLAGS} -I/usr/include/sparsehash -I/usr/include/google"]
AC_CHECK_HEADER([dense_hash_set], [],
                [AC_MSG_ERROR([sparsehash not found. Run: sudo apt-get install libsparsehash-dev])])

AC_CHECK_LIB([event], [evhttp_start], [],
             [AC_MSG_ERROR([libevent not found. Run: sudo apt-get install libevent-dev])])

AC_ARG_WITH(store, [  --with-store=STORE      choose store type: vector (default), hash])
if test "x${with_store}" != "xhash"; then
  AC_DEFINE(VECTOR_STORE, 1)
fi

AC_CONFIG_FILES([Makefile src/madstored/Makefile])
AC_OUTPUT
