#pragma once

#include "functions.h"

int my_strcmp(char *a, char *b);
int my_memcmp(u8 *a, u8 *b, int size);
void my_memcpy(u8 *to, const u8 *from, int size);
char *my_strcpy_till(char *to, const char *from, char till);
void my_strcpy(char *to, const char *from);
char *my_strcat_till(char *to, const char *from, char till);
void my_strcat(char *to, const char *from);
char *my_strstr(char *haystack, char needle);
