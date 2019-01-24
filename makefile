LIBS = -lm -lSDL2
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi $(LIBS)
DEBUG = -g3
OPTIM = -O2
VALGRIND = --error-exitcode=1 --quiet --leak-check=full
SPELLLIST = eng_370k_shuffle.txt
BOOK = heart_darkness.txt

all: prstst interptst exttst

prstst: preader.c parser.c parser.h
	$(CC) preader.c parser.c -o prstst $(CFLAGS) $(OPTIM) -include parser.h

interptst: ireader.c interpreter.c interpreter.h
	$(CC) ireader.c interpreter.c neillsdl2.c neillsdl2.h -o interptst $(CFLAGS) $(OPTIM) -include interpreter.h

exttst: ireader.c extension.c extension.h
	$(CC) ireader.c extension.c neillsdl2.c neillsdl2.h hsh.c -o exttst $(CFLAGS) $(OPTIM) -include extension.h

run: all
	./prstst test.turt
	./interptst test.turt
	./exttst extension.turt

clean:
	rm -f interptst prstst exttst

.PHONY: clean run all
