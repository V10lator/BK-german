#include "strhelper.h"

int my_strcmp(char *a, char *b)
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

int my_memcmp(u8 *a, u8 *b, int size)
{
    for(int i = 0; i < size; i++)
        if(a[i] != b[i])
            return 0;

    return 1;
}

char *my_strcpy_till(char *to, char *from, char till)
{
    while(*from != till)
        *to++ = *from++;

    *to = '\0';
    return from;
}

void my_strcpy(char *to, char *from)
{
    my_strcpy_till(to, from, '\0');
};

char *my_strcat_till(char *to, char *from, char till)
{
    while(*to != '\0')
        to++;

    return my_strcpy_till(to, from, till);
}

void my_strcat(char *to, char *from)
{
    while(*to != '\0')
        to++;

    my_strcpy(to, from);
}

char *my_strstr(char *haystack, char needle)
{
    while(*haystack != needle)
    {
        if(*haystack == '\0')
            return NULL;

        haystack++;
    }

    return haystack;
}
