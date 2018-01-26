#Turner Robbins
all: mc2 tests

mc2: main.c util.h util.c
	gcc -o mc2 main.c util.c

tests: test_midday_commander.c util.h util.c
	gcc -g -o tests test_midday_commander.c util.c

clean:
	rm -f mc2 tests main.o
