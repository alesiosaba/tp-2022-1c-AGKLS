# Original project path
PROJ_PATH=../../kernel

# Excluded source files (eg: main() function)
EXCLUDE=kernel.c

# Libraries
LIBS=commons shared readline pthread cunit

# Custom libraries' paths
SHARED_LIBPATHS= /home/utnso/shared/tp-2022-1c-AGKLS/shared/bin
STATIC_LIBPATHS=

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"
