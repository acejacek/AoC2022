OUTPUT = day1

CFLAGS = -Wall -Werror -Wextra -pedantic
# LFLAGS = -lm

all: CFLAGS += -O2
all: ${OUTPUT}

debug: CFLAGS += -DDEBUG -g
debug: exec

run:	${OUTPUT}
	./${OUTPUT}

day1: day1.c
	cc ${CFLAGS} -o day1 day1.c ${LFLAGS}

clean:
	rm -f *.o
	rm -rf *.dSYM
