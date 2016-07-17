bin_PROGRAMS = madstored
madstored_SOURCES = logger.cc rest_api.cc madstored.cc
madstored_CPPFLAGS = -I../lib $(luajit_CFLAGS)
madstored_LDADD = $(MADSTORED_LDADD) \
									$(luajit_LIBS) \
									../lib/libmadstore.a
