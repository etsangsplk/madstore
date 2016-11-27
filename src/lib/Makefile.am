EXTRA_DIST = store_factory.cc.j2 \
						 gen_sources.py

noinst_LIBRARIES = libmadstore.a
libmadstore_a_SOURCES = input_spec.cc \
												store_spec.cc \
												store_facade.cc \
												murmur3.cc
nodist_libmadstore_a_SOURCES = store_factory.cc

if EXPRESSIONS
libmadstore_a_SOURCES += lua.cc
endif
libmadstore_a_CPPFLAGS = $(luajit_CFLAGS)

store_factory.cc: $(srcdir)/store_factory.cc.j2  $(srcdir)/gen_sources.py
	./gen_sources.py $(DIMS) $(METRICS)

clean-local:
	-rm -f store_factory.cc

