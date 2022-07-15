#Libraries
#LIBS = -lcommons -lshared -lreadline -lpthread -lcunit  

# Custom Libraries paths
#SHARED_LIBPATHS = -L"/home/utnso/shared/tp-2022-1c-AGKLS/shared/bin"


# Libraries
LIBS= commons shared readline

# Custom libraries' paths
SHARED_LIBPATHS= ../shared
STATIC_LIBPATHS=

# Compiler flags
CDEBUG=-g -Wall -DDEBUG
CRELEASE=-O3 -Wall -DNDEBUG

# Arguments when executing with start, memcheck or helgrind
ARGS=

# Valgrind flags
MEMCHECK_FLAGS=--track-origins=yes --log-file="memcheck.log"
HELGRIND_FLAGS=--log-file="helgrind.log"