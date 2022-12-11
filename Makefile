OUTPUT = day1

BINDIR = ./bin/

CFLAGS = -Wall -Werror -Wextra -pedantic
# LFLAGS = -lm

all: CFLAGS += -O2
all: ${OUTPUT}

debug: CFLAGS += -DDEBUG -g
debug: exec

run:	${OUTPUT}
	${BINDIR}${OUTPUT}

day1: day1.c
	cc ${CFLAGS} -o ${BINDIR}day1 day1.c ${LFLAGS}

clean:
	rm -f *.o
	rm -rf *.dSYM
