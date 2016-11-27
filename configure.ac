AC_INIT([madstore], [0.1])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_PROG_RANLIB
AC_LANG([C++])
: ${CXXFLAGS=-O3}
AC_PROG_CXX
AC_SUBST([CC])
AX_CXX_COMPILE_STDCXX_14

AM_PATH_PYTHON([2.7])

[CPPFLAGS="${CPPFLAGS} -DELPP_NO_DEFAULT_LOG_FILE -DELPP_THREAD_SAFE"]
[LDFLAGS="${LDFLAGS} -static-libstdc++ -static-libgcc"]

[CPPFLAGS="${CPPFLAGS} -I/usr/include/sparsehash -I/usr/include/google"]
AC_CHECK_HEADER([dense_hash_set], [],
                [AC_MSG_ERROR([sparsehash not found. Run: sudo apt-get install libsparsehash-dev])])

AC_CHECK_LIB([boost_system], [main], [],
             [AC_MSG_ERROR([boost system not found. Run: sudo apt-get install libboost-system-dev])])

AC_CHECK_LIB([boost_thread], [main], [],
             [AC_MSG_ERROR([boost thread not found. Run: sudo apt-get install libboost-thread-dev])])

AC_CHECK_LIB([boost_filesystem], [main], [],
             [AC_MSG_ERROR([boost filesystem not found. Run: sudo apt-get install libboost-filesystem-dev])])

AC_CHECK_LIB([boost_date_time], [main], [],
             [AC_MSG_ERROR([boost date-time not found. Run: sudo apt-get install libboost-date-time-dev])])

AC_SUBST([MADSTORED_LDADD], [-pthread])

AC_ARG_ENABLE([expressions],
              AS_HELP_STRING([--enable-expressions], [enable support for expressions in select]))
AS_IF([test "x$enable_expressions" = "xyes"], [
  PKG_CHECK_MODULES([luajit], [luajit >= 2.0])
  AC_DEFINE([EXPRESSIONS], [1])
  AM_CONDITIONAL([EXPRESSIONS], [true])
], [
  AM_CONDITIONAL([EXPRESSIONS], [false])
])

AC_ARG_WITH([dims],
            AS_HELP_STRING([--with-dims=NUM], [number of dimensions (default: 10)]),
            [DIMS="$withval"], [DIMS=10])
AC_SUBST(DIMS)

AC_ARG_WITH([metrics],
            AS_HELP_STRING([--with-metrics=NUM], [number of metrics (default: 5)]),
            [METRICS="$withval"], [METRICS=5])
AC_SUBST(METRICS)

AC_ARG_ENABLE([persistence],
              AS_HELP_STRING([--enable-persistence], [enable saving memory snapshot to disk]))
AS_IF([test "x$enable_persistence" = "xyes"], [
  AC_DEFINE([PERSIST], [1])
])

AC_ARG_ENABLE([debug],
              AS_HELP_STRING([--enable-debug], [disable optimizations and enable debug symbols]))
AS_IF([test "x$enable_debug" = "xyes"], [
  AC_SUBST([CXXFLAGS], [-g])
])

AC_CONFIG_FILES([Makefile src/lib/Makefile src/madstored/Makefile])
AC_OUTPUT
