#pragma once

#include "functions.h"

extern int strcmp(const char *a, const char *b);
extern int memcmp(const u8 *a, const u8 *b, int size);
char *strcpy_till(char *to, const char *from, char till);
char *strcat_till(char *to, const char *from, char till);
extern char *strstr(const char *haystack, const char *needle);
char *strstrc(const char *haystack, char needle);
