OUTPUT = day12

BINDIR = ./bin/

CFLAGS = -Wall -Werror -Wextra -pedantic
# LFLAGS = -lm

all: CFLAGS += -O2
all: ${OUTPUT}

debug: CFLAGS += -DDEBUG -g
debug: ${OUTPUT}

run:	${OUTPUT}
	${BINDIR}${OUTPUT}

${OUTPUT}: ${OUTPUT}.c
	cc ${CFLAGS} -o ${BINDIR}${OUTPUT} ${OUTPUT}.c ${LFLAGS}

clean:
	rm -f *.o ${BINDIR}${OUTPUT}
	rm -rf *.dSYM
