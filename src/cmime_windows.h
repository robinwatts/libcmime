#ifndef CMIME_WINDOWS_H_
#define CMIME_WINDOWS_H_

#include <stdarg.h>

#if defined(WIN32) || defined(_WIN32) || defined(WIN64) || defined(_WIN64)
#ifdef __cplusplus
extern "C"
{
#endif

#include <winsock.h>
#include <sys/stat.h>
#include <stdio.h>

#define strdup _strdup
#define fileno _fileno
#define fdopen _fdopen
#define P_tmpdir "I_THINK_THIS_IS_UNUSED_FOR_WHAT_WE_NEED"
#define restrict __restrict

#define MAXHOSTNAMELEN 256


int vasprintf(char** ptr, const char* format, va_list ap);
int asprintf(char** ptr, const char* format, ...);

char *strcasestr(const char *big, const char *little);
char *strsep(char **restrict stringp, const char *restrict delim);
int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *string1, const char *string2, size_t count);
char *strtok_r(char *str, const char *delim, char **saveptr);
char *basename(char *path);
FILE *popen(const char *command, const char *mode);
int pclose(FILE *stream);
int mkstemp(char *template);
int getpid(void);
size_t getline(char **restrict lineptr, size_t *restrict n, FILE *restrict stream);
int rand_r(unsigned int *seedp);

#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif

#ifdef __cplusplus
}
#endif

#else
/* Non windows platforms */
#include <sys/mman.h>
#include <unistd.h>
#include <sys/param.h>
#include <libgen.h>

#endif

#endif
