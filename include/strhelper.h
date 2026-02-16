#pragma once

#include "functions.h"

int strcmp(char *a, char *b);
int memcmp(u8 *a, u8 *b, int size);
//void memcpy(u8 *to, const u8 *from, int size);
char *strcpy_till(char *to, const char *from, char till);
//void strcpy(char *to, const char *from);
char *strcat_till(char *to, const char *from, char till);
//void strcat(char *to, const char *from);
char *strstr(char *haystack, char needle);
