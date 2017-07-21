#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strdupNEW(const char *s) {
	char *d =(char*) malloc(strlen(s) + 1);   // Allocate memory
	if (d != NULL) strcpy(d, s);         // Copy string if okay
	return d;                            // Return new memory
}
