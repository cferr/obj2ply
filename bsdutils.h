#ifndef _BSDUTILS_H
#define _BSDUTILS_H

#include <sys/types.h>
#include <stdio.h>

#ifdef _WIN32
typedef long long int ssize_t;
#endif

ssize_t
getdelim(char **buf, size_t *bufsiz, int delimiter, FILE *fp);

ssize_t
getline(char **buf, size_t *bufsiz, FILE *fp);

#endif
