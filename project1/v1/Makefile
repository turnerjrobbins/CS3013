#Turner Robbins
all: mc1 tests

mc1: main.c util.h
	gcc -o mc1 main.c

tests: test_midday_commander.c
	gcc -o tests test_midday_commander.c

clean:
	rm -f mc1 tests main.o
