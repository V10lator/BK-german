#pragma once

#include "functions.h"

int my_strcmp(char *a, char *b);
int my_memcmp(u8 *a, u8 *b, int size);
char *my_strcpy_till(char *to, char *from, char till);
void my_strcpy(char *to, char *from);
char *my_strcat_till(char *to, char *from, char till);
void my_strcat(char *to, char *from);
char *my_strstr(char *haystack, char needle);
