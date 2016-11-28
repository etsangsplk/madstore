EXTRA_DIST = table_factory.cc.j2 \
						 gen_sources.py

noinst_LIBRARIES = libmadstore.a
libmadstore_a_SOURCES = specs.cc \
												store.cc \
												murmur3.cc
nodist_libmadstore_a_SOURCES = table_factory.cc

if EXPRESSIONS
libmadstore_a_SOURCES += lua.cc
endif
libmadstore_a_CPPFLAGS = $(luajit_CFLAGS)

table_factory.cc: $(srcdir)/table_factory.cc.j2  $(srcdir)/gen_sources.py
	./gen_sources.py $(TABLES)

clean-local:
	-rm -f table_factory.cc

