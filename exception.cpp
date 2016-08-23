#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "exception.h"

Exception :: Exception()
{
    message = NULL;
}

Exception :: Exception(const char *s)
{
    if (s == NULL)
    {
        message = NULL;
        return;
    }
    message = (char *)malloc(strlen(s) + 1);
    if (message != NULL)
        memcpy(message, s, strlen(s));
    message[strlen(s)] = '\0';
    
}

Exception :: Exception(const Exception &x)
{
    if (x.message == NULL)
    {
        this->message = NULL;
        return;
    }
    this->message = (char *)malloc(strlen(x.message) + 1);
    if (this->message != NULL)
        strcpy(this->message, x.message);
    message[strlen(x.message)] = '\0';
}

Exception :: ~Exception()
{
    if (message != NULL)
        free(message);
}

void Exception :: print(FILE *f)
{
    fprintf(f, "%s\n",message);
}

