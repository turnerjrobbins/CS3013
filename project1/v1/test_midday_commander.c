//test_midday_commander.c

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

int main() {
	//assert(false && "Failing unit test");
	char *testStr = malloc(128); 
	strcpy(testStr, "echo hello\n");
	char *resultStr = malloc(128);
	strcpy(resultStr, "echo hello ");
	stripNewlineAddSpace(&testStr, 128);
	assert(strcmp(testStr, resultStr)==0 && "stripNewLineAddSpace is broken");
}
