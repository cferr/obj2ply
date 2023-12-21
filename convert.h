#ifndef _CONVERT_H
#define _CONVERT_H

#include "bsdutils.h"

#ifdef _WIN32
#include <windows.h>
typedef wchar_t* str_t;
#define FOPEN _wfopen
#else
typedef char* str_t;
#define FOPEN fopen
#endif

int convert(const str_t f_in_name, const str_t f_out_name);

#endif