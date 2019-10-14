# GLOBAL CFLAGS
AM_CFLAGS   = -O2 -Wall
AM_CXXFLAGS = $(CFLAGS)

bin_PROGRAMS = fatracer
fatracer_SOURCES = src/main.c src/fat12_common.c src/fat32_common.c
if DEBUG
fatracer_CFLAGS = -O0 -g3 -coverage -Wall -DFATRACER_DEBUG
else
fatracer_CFLAGS = -O2
endif

EXTRA_DIST = docs man
man_MANS = man/fatracer.1
TESTS = tests/simple.sh
