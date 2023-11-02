#include <errno.h>  
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cmime_windows.h"

// This asprintf and vasprintf implementation was lifted from:
// https://github.com/eiszapfen2000/asprintf
// It is under the following license:
//
// BSD 3-Clause License
//
// Copyright (c) 2018, Thomas Gamper
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#if _MSC_VER < 1800
#undef va_copy
#define va_copy(dst, src) (dst = src)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
int vasprintf(char** strp, const char* fmt, va_list ap)
{
    va_list ap_copy;
    int formattedLength, actualLength;
    size_t requiredSize;

    // be paranoid
    *strp = NULL;

    // copy va_list, as it is used twice 
    va_copy(ap_copy, ap);

    // compute length of formatted string, without NULL terminator
    formattedLength = _vscprintf(fmt, ap_copy);
    va_end(ap_copy);

    // bail out on error
    if (formattedLength < 0)
    {
        return -1;
    }

    // allocate buffer, with NULL terminator
    requiredSize = ((size_t)formattedLength) + 1;
    *strp = (char*)malloc(requiredSize);

    // bail out on failed memory allocation
    if (*strp == NULL)
    {
        errno = ENOMEM;
        return -1;
    }

    // write formatted string to buffer, use security hardened _s function
    actualLength = vsnprintf_s(*strp, requiredSize, requiredSize - 1, fmt, ap);

    // again, be paranoid
    if (actualLength != formattedLength)
    {
        free(*strp);
        *strp = NULL;
        errno = EOTHER;
        return -1;
    }

    return formattedLength;
}

int asprintf(char** strp, const char* fmt, ...)
{
    int result;

    va_list ap;
    va_start(ap, fmt);
    result = vasprintf(strp, fmt, ap);
    va_end(ap);

    return result;
}

static inline
char my_to_upper(char c)
{
    if (c >= 'a' && c <= 'z')
        c += 'A' - 'a';
    return c;
}

char *strcasestr(const char *big, const char *little)
{
    char c;
    char f = my_to_upper(*little++);
    size_t z;

    if (f == 0)
        return big;
    z = strlen(little);

    while ((c = *big++) != 0)
    {
        c = my_to_upper(c);
        if (c == f && strncasecmp(big, little, z) == 0)
            return big-1;
    }

    return NULL;
}

char *strsep(char **restrict stringp, const char *restrict delim)
{
    char c;
    char *str = *stringp;
    char *start = str;

    if (str == NULL)
        return NULL;

    while ((c = *str++) != 0)
    {
        char d;
        const char *del = delim;
        while ((d = *del++) != 0)
        {
            if (c == d)
            {
                str[-1] = 0;
                *stringp = str;
                return start;
            }
        }
    }

    return NULL;
}

int strcasecmp(const char *s1, const char *s2)
{
    while (1)
    {
        char a = my_to_upper(*s1++);
        char b = my_to_upper(*s2++);

        if (a == b)
        {
            if (a == 0)
                return 0;
        }
        else
            return a-b;
    }
}

int strncasecmp(const char *s1, const char *s2, size_t count)
{
    while (count--)
    {
        char a = my_to_upper(*s1++);
        char b = my_to_upper(*s2++);

        if (a == b)
        {
            if (a == 0)
                return 0;
        }
        else
            return a-b;
    }
    return 0;
}

char *strtok_r(char *str, const char *delim, char **saveptr)
{
    assert(!"strtok_r");
    return NULL;
}

char *basename(char *path)
{
    assert(!"basename");
    return NULL;
}

FILE *popen(const char *command, const char *mode)
{
    assert(!"popen");
    return NULL;
}

int pclose(FILE *stream)
{
    assert(!"pclose");
    return 0;
}

int mkstemp(char *template)
{
    assert(!"mkstemp");
    return 0;
}

int getpid(void)
{
    return 0;
}

size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream)
{
    assert(!"getline");
    return 0;
}

int rand_r(unsigned int *seedp)
{
    return 0;
}

int gethostname(char *name, int namelen)
{
    assert(!"gethostname");
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
