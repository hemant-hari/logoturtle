LIBS = -lm
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi $(LIBS)
DEBUG = -g3
OPTIM = -O2
VALGRIND = --error-exitcode=1 --quiet --leak-check=full
SPELLLIST = eng_370k_shuffle.txt
BOOK = heart_darkness.txt

all: prstst splhsh

prstst: test.c parser.c parser.h
	$(CC) test.c parser.c -o prstst $(CFLAGS) $(OPTIM) -include parser.h

splhsh: spl.c hsh.c hsh.h
	$(CC) spl.c hsh.c -o splhsh $(CFLAGS) $(OPTIM) -include hsh.h

run: all
	./splbst $(SPELLLIST) $(BOOK)
	./splhsh $(SPELLLIST) $(BOOK)

clean:
	rm -f splbst splhsh

.PHONY: clean run all
