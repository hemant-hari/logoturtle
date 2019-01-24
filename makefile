LIBS = -lm -lSDL2
CFLAGS = -Wall -Wextra -Werror -Wfloat-equal -pedantic -ansi $(LIBS)
DEBUG = -g3
OPTIM = -O2
VALGRIND = --error-exitcode=1 --quiet --leak-check=full
SPELLLIST = eng_370k_shuffle.txt
BOOK = heart_darkness.txt

all: parse interpret extension

parse: preader.c parser.c parser.h
	$(CC) preader.c parser.c -o parse $(CFLAGS) $(OPTIM) -include parser.h

interpret: ireader.c interpreter.c interpreter.h
	$(CC) ireader.c interpreter.c neillsdl2.c neillsdl2.h -o interpret $(CFLAGS) $(OPTIM) -include interpreter.h

extension: ireader.c extension.c extension.h
	$(CC) ireader.c extension.c neillsdl2.c neillsdl2.h hsh.c -o extension $(CFLAGS) $(OPTIM) -include extension.h

run: all
	./parse test.turt
	./interpret test.turt
	./extension extension.turt

clean:
	rm -f interpret parse extension

.PHONY: clean run all
