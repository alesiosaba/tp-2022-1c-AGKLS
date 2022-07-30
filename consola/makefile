include settings.mk

################################################################################

filename = $(1).out

define compile_bin
	gcc -w $(CFLAGS) -o "$@" $^ $(IDIRS:%=-I%) $(LIBDIRS:%=-L%) $(LIBS:%=-l%)
endef

define compile_objs
	gcc -w $(CFLAGS) -c -o "$@" $< $(IDIRS:%=-I%)
endef

################################################################################

include ../compilation.mk

################################################################################

include ../execution.mk

