#Turner Robbins
all: mc1

mc1: main.c util.h util.c
	gcc -o mc1 main.c util.c

tests: test_midday_commander.c util.h util.c
	gcc -g -o tests test_midday_commander.c util.c

clean:
	rm -f mc1 tests main.o
