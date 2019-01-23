LIBS = -lm -lSDL2
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi $(LIBS)
DEBUG = -g3
OPTIM = -O2
VALGRIND = --error-exitcode=1 --quiet --leak-check=full
SPELLLIST = eng_370k_shuffle.txt
BOOK = heart_darkness.txt

all: prstst interptst exttst

prstst: tstprs.c parser.c parser.h
	$(CC) tstprs.c parser.c -o prstst $(CFLAGS) $(OPTIM) -include parser.h

interptst: test.c interpreter.c interpreter.h
	$(CC) test.c interpreter.c neillsdl2.c neillsdl2.h -o interptst $(CFLAGS) $(OPTIM) -include interpreter.h

exttst: test.c extension.c extension.h
	$(CC) test.c extension.c neillsdl2.c neillsdl2.h hsh.c -o exttst $(CFLAGS) $(OPTIM) -include extension.h

run: all
	./prstst test.turt
	./interptst test.turt
	./exttst extension.turt

clean:
	rm -f interptst prstst exttst

.PHONY: clean run all
