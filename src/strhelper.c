#include "strhelper.h"

int strcmp(char *a, char *b)
{
    do
    {
        if(*a != *b)
            return 0;

        a++;
        b++;
    } while(*a != '\0' || *b != '\0');

    return 1;
}

int memcmp(u8 *a, u8 *b, int size)
{
    for(int i = 0; i < size; i++)
        if(a[i] != b[i])
            return 0;

    return 1;
}
/*
void memcpy(u8 *to, const u8 *from, int size)
{
    for(int i = 0; i < size; i++)
        to[i] = from[i];
}
*/
char *strcpy_till(char *to, const char *from, char till)
{
    while(*from != till)
        *to++ = *from++;

    *to = '\0';
    return (char *)from;
}
/*
void strcpy(char *to, const char *from)
{
    my_strcpy_till(to, from, '\0');
};
*/
char *strcat_till(char *to, const char *from, char till)
{
    while(*to != '\0')
        to++;

    return strcpy_till(to, from, till);
}
/*
void strcat(char *to, const char *from)
{
    while(*to != '\0')
        to++;

    strcpy(to, from);
}
*/
char *strstr(char *haystack, char needle)
{
    while(*haystack != needle)
    {
        if(*haystack == '\0')
            return NULL;

        haystack++;
    }

    return haystack;
}
