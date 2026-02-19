#include "strhelper.h"

char *strcpy_till(char *to, const char *from, char till)
{
    while(*from != till)
        *to++ = *from++;

    *to = '\0';
    return (char *)from;
}

char *strcat_till(char *to, const char *from, char till)
{
    while(*to != '\0')
        to++;

    return strcpy_till(to, from, till);
}

char *strstrc(const char *haystack, char needle)
{
    while(*haystack != needle)
    {
        if(*haystack == '\0')
            return NULL;

        haystack++;
    }

    return (char *)haystack;
}
