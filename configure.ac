AC_INIT([madstore], [0.1])

AM_INIT_AUTOMAKE
AC_PROG_RANLIB
AC_LANG([C++])
AC_PROG_CXX
AC_SUBST([CC])

[CPPFLAGS="${CPPFLAGS} -std=c++11"]
[CPPFLAGS="${CPPFLAGS} -I/usr/include/sparsehash -I/usr/include/google"]
AC_CHECK_HEADER([dense_hash_set], [], [AC_MSG_ERROR([sparsehash headers not found])])

AC_CONFIG_FILES([Makefile src/lib/Makefile src/madstored/Makefile])
AC_OUTPUT
