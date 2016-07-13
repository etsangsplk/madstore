noinst_LIBRARIES = libmadstore.a
libmadstore_a_SOURCES = input_spec.cc \
                        store_spec.cc \
                        store_factory.cc \
                        store_facade.cc
if JIT
libmadstore_a_SOURCES += lisp.cc \
												 jit.cc 
endif

