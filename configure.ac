AC_INIT([madstore], [0.1])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_PROG_RANLIB
AC_LANG([C++])
: ${CXXFLAGS=-O3}
AC_PROG_CXX
AC_SUBST([CC])
AX_CXX_COMPILE_STDCXX_14

[CPPFLAGS="${CPPFLAGS} -DELPP_NO_DEFAULT_LOG_FILE"]
[LDFLAGS="${LDFLAGS} -static-libstdc++ -static-libgcc"]

[CPPFLAGS="${CPPFLAGS} -I/usr/include/sparsehash -I/usr/include/google"]
AC_CHECK_HEADER([dense_hash_set], [],
                [AC_MSG_ERROR([sparsehash not found. Run: sudo apt-get install libsparsehash-dev])])

AC_CHECK_LIB([event], [evhttp_start], [],
             [AC_MSG_ERROR([libevent not found. Run: sudo apt-get install libevent-dev])])

AC_ARG_ENABLE([expressions],
              AS_HELP_STRING([--enable-expressions], [enable support for expressions in select]))
AS_IF([test "x$enable_expressions" = "xyes"], [
  AC_DEFINE([JIT], [1])
  AC_CHECK_HEADER([asmjit/asmjit.h], [],
               [AC_MSG_ERROR([asmjit not found. Go to this Website to install it: https://github.com/kobalicek/asmjit])])
  AC_CHECK_LIB([asmjit], [main], [],
               [AC_MSG_ERROR([asmjit not found. Go to this Website to install it: https://github.com/kobalicek/asmjit])])
  AC_SUBST([MADSTORED_LDADD], [-lasmjit])
])
AM_CONDITIONAL([JIT], [test "x$enable_expressions" = "xyes"])

AC_ARG_WITH([dims-x],
            AS_HELP_STRING([--with-dims-x=NUM], [allowed dimensions number multiplier]))
AS_IF([test ! -z "${with_dims_x}"], [AC_DEFINE_UNQUOTED([DIMS_X], [$with_dims_x])])

AC_ARG_WITH([metrics-x],
            AS_HELP_STRING([--with-metrics-x=NUM], [allowed metrics number multiplier]))
AS_IF([test ! -z "${with_metrics_x}"], [AC_DEFINE_UNQUOTED([METRICS_X], [$with_metrics_x])])

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

AC_CONFIG_FILES([Makefile src/lib/Makefile src/madstored/Makefile])
AC_OUTPUT
