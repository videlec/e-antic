# Installed library (libtool (LT) libraries)
lib_LTLIBRARIES = libeanticxx.la
libeanticxx_la_LDFLAGS = -version-info @libeanticxx_version_info@ -no-undefined

# Linked-against libraries
libeanticxx_la_LIBADD = ../src/libeantic.la -lgmpxx -lflint -lantic

# Installed headers
nobase_pkginclude_HEADERS =       \
    ../e-antic/cppyy.h         \
    ../e-antic/renfxx.h        \
    ../e-antic/renfxx_fwd.h    \
    ../e-antic/renfxx_cereal.h

libeanticxx_la_SOURCES = \
    renf_class.cpp       \
    renf_elem_class.cpp

noinst_HEADERS =                               \
    external/unique-factory/unique_factory.hpp
