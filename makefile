LIBS = -lm -lSDL2main -lSDL2
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi $(LIBS)
DEBUG = -g3
OPTIM = -O2
VALGRIND = --error-exitcode=1 --quiet --leak-check=full
SPELLLIST = eng_370k_shuffle.txt
BOOK = heart_darkness.txt

all: prstst interptst

prstst: tstprs.c parser.c parser.h
	$(CC) tstprs.c parser.c -o prstst $(CFLAGS) $(OPTIM) -include parser.h

interptst: test.c interpreter.c interpreter.h
	$(CC) test.c interpreter.c neillsdl2.c neillsdl2.h -o interptst $(CFLAGS) $(OPTIM) -include interpreter.h

splhsh: spl.c hsh.c hsh.h
	$(CC) spl.c hsh.c -o splhsh $(CFLAGS) $(OPTIM) -include hsh.h

run: all
	./prstst test.turt
	./interptst test.turt

clean:
	rm -f splbst splhsh

.PHONY: clean run all
