#ifndef _BSDUTILS_H

#include <sys/types.h>
#include <stdio.h>

#ifdef _WIN32
typedef ssize_t long long int;
#endif

ssize_t
getdelim(char **buf, size_t *bufsiz, int delimiter, FILE *fp);

ssize_t
getline(char **buf, size_t *bufsiz, FILE *fp);

#endif
