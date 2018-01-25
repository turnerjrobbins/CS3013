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
	replaceNewline(&testStr, 128, ' ');
	assert(strcmp(testStr, resultStr)==0 && "stripNewLineAddSpace is broken");

	char *withoutAmpersand[] = {"sleep", "10", NULL};
	char *withAmpersand[] = {"sleep", "10&", NULL};
	int rc = containsAmpersand(withAmpersand);
	assert(rc == 1 && "contains ampersand with ampersand failed");
	rc = containsAmpersand(withoutAmpersand);
	assert(rc == 0 && "contains ampersand without ampersand failed"); 

	//adding and removing
	assert(removeProcess(1000) == -1 && "Shouldn't be able to remove empty process");
	assert(addProcess(1000) == 0 && "error adding process");
	assert(getProcessList()[0] == 1000 && "Not added correctly");
}
