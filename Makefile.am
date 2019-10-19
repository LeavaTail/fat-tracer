# GLOBAL CFLAGS
AM_CFLAGS   = -O2 -Wall
AM_CXXFLAGS = $(CFLAGS)

bin_PROGRAMS = fatracer
fatracer_SOURCES = src/main.c src/fat12_common.c src/fat16_common.c src/fat32_common.c

fatracer_CFLAGS = -DLOCALEDIR='"$(localedir)"'
if DEBUG
fatracer_CFLAGS += -O0 -g3 -coverage -Wall -DFATRACER_DEBUG
else
fatracer_CFLAGS += -O2
endif

EXTRA_DIST = docs man
man_MANS = man/fatracer.1

ACLOCAL_AMFLAGS = -I ./m4
SUBDIRS = intl po

TESTS = tests/simple.sh tests/usage.sh
