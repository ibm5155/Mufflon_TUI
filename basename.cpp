#pragma once
#ifndef LIBGEN_H

#define LIBGEN_H
#include <stdlib.h>
#include <string.h>

char *basename(char *path)

{

	char *s = strrchr(path, '/');



	return (s == NULL) ? path : ++s;

}



#endif /* LIBGEN_H */