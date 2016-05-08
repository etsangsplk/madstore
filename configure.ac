AC_INIT([madstore], [0.1])

AM_INIT_AUTOMAKE
AC_PROG_RANLIB
AC_LANG([C++])
: ${CXXFLAGS=-O3}
AC_PROG_CXX
AC_SUBST([CC])

[CPPFLAGS="${CPPFLAGS} -std=c++14 -DELPP_NO_DEFAULT_LOG_FILE"]
[LDFLAGS="${LDFLAGS} -static-libstdc++ -static-libgcc"]

[CPPFLAGS="${CPPFLAGS} -I/usr/include/sparsehash -I/usr/include/google"]
AC_CHECK_HEADER([dense_hash_set], [],
                [AC_MSG_ERROR([sparsehash not found. Run: sudo apt-get install libsparsehash-dev])])

AC_CHECK_LIB([event], [evhttp_start], [],
             [AC_MSG_ERROR([libevent not found. Run: sudo apt-get install libevent-dev])])

AC_ARG_WITH([dims-x],
            AS_HELP_STRING([--with-dims-x=NUM], [allowed dimensions number multiplier]))
AS_IF([test ! -z "x${with_dims_x}"], [AC_DEFINE_UNQUOTED([DIMS_X], [$with_dims_x])])

AC_ARG_WITH([metrics-x],
            AS_HELP_STRING([--with-metrics-x=NUM], [allowed metrics number multiplier]))
AS_IF([test ! -z "x${with_metrics_x}"], [AC_DEFINE_UNQUOTED([METRICS_X], [$with_metrics_x])])

AC_ARG_ENABLE([persistence],
              AS_HELP_STRING([--enable-persistence], [enable saving memory snapshot to disk]))
AS_IF([test "x$enable_persistence" = "xyes"], [
  AC_DEFINE([PERSIST], [1])
  AC_CHECK_LIB([boost_system], [main], [],
              [AC_MSG_ERROR([boost not found. Run: sudo apt-get install libboost-dev])])
  AC_SUBST([MADSTORED_LDADD], [-pthread])
])

AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug], [disable optimizations and enable debug symbols]))
AS_IF([test "x$enable_debug" = "xyes"], [
  AC_SUBST([CXXFLAGS], [-g])
])

AC_CONFIG_FILES([Makefile src/include/Makefile src/madstored/Makefile])
AC_OUTPUT
