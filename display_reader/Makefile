TARGET  := display_reader
SRCS    := main.c
OBJS    := ${SRCS:.c=.o} 
DEPS    := ${SRCS:.c=.dep} 
XDEPS   := $(wildcard ${DEPS}) 
CCFLAGS = -std=gnu99 -O2 -Wall -ggdb -Wextra -pedantic
LDFLAGS = 
LIBS    = $(shell imlib2-config --libs)


.PHONY: all clean distclean 
all:: ${TARGET} 

ifneq (${XDEPS},) 
include ${XDEPS} 
endif 

${TARGET}: ${OBJS} 
	${CC} ${LDFLAGS} -o $@ $^ ${LIBS} 

${OBJS}: %.o: %.c %.dep 
	${CC} ${CCFLAGS} -o $@ -c $< 

${DEPS}: %.dep: %.c Makefile 
	${CC} ${CCFLAGS} -MM $< > $@ 

clean:: 
	-rm -f *~ *.o ${TARGET} 

distclean:: clean

