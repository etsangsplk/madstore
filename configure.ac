AC_INIT([madstore], [0.1])

AM_INIT_AUTOMAKE
AC_PROG_RANLIB
AC_LANG([C++])
: ${CXXFLAGS=-O3}
AC_PROG_CXX
AC_SUBST([CC])

[CPPFLAGS="${CPPFLAGS} -std=c++11"]
[LDFLAGS="${LDFLAGS} -static-libstdc++ -static-libgcc"]

[CPPFLAGS="${CPPFLAGS} -I/usr/include/sparsehash -I/usr/include/google"]
AC_CHECK_HEADER([dense_hash_set], [],
                [AC_MSG_ERROR([sparsehash not found. Run: sudo apt-get install libsparsehash-dev])])

AC_CHECK_LIB([event], [evhttp_start], [],
             [AC_MSG_ERROR([libevent not found. Run: sudo apt-get install libevent-dev])])

AC_ARG_WITH(dims-x, [  --with-dims-x=NUM       allowed dimensions number multiplier])
if test ! -z "x${with_dims_x}"; then
  AC_DEFINE_UNQUOTED([DIMS_X], [$with_dims_x])
fi

AC_ARG_WITH(metrics-x, [  --with-metrics-x=NUM    allowed metrics number multiplier])
if test ! -z "x${with_metrics_x}"; then
  AC_DEFINE_UNQUOTED([METRICS_X], [$with_metrics_x])
fi

AC_CONFIG_FILES([Makefile src/madstored/Makefile])
AC_OUTPUT
