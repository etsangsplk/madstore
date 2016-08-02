noinst_LIBRARIES = libmadstore.a
libmadstore_a_SOURCES = input_spec.cc \
												store_spec.cc \
												store_factory.cc \
												store_facade.cc \
												murmur3.cc
if EXPRESSIONS
libmadstore_a_SOURCES += lua.cc
endif
libmadstore_a_CPPFLAGS = $(luajit_CFLAGS)
