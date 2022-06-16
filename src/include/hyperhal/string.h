#ifndef STRING_H
#define STRING_H
#include <types.h>

#ifdef __cplusplus
extern "C" {
int         strcmp (const char*, const char*);
int         strncmp(const char*, const char*, size_t);
char*       strcpy (      char*, const char*);
char*       strncpy(      char*, const char*, size_t);
char*       strcat (      char*, const char*);
char*       strncat(      char*, const char*, size_t);
const char* strchr (const char*, char);
char*       strrev (      char*);
size_t      strlen (const char*);
size_t utf8_strlen (const uint32_t*);
char*       strtok (      char *, const char*);

void  itoa(int, char*);
int   atoi(const char *);
int   memcmp(const void*, const void*, size_t);
void* memmove(void* , void* , size_t);
void* memset(void*, uint8_t, size_t);
void* memcpy(void*, const void*, size_t);
}
#else
int         strcmp (const char*, const char*);
int         strncmp(const char*, const char*, size_t);
char*       strcpy (      char*, const char*);
char*       strncpy(      char*, const char*, size_t);
char*       strcat (      char*, const char*);
char*       strncat(      char*, const char*, size_t);
const char* strchr (const char*, char);
char*       strrev (      char*);
size_t      strlen (const char*);
size_t utf8_strlen (const uint32_t*);
char*       strtok (      char *, const char*);

void  itoa(int, char*);
int   atoi(const char *);
int   memcmp(const void*, const void*, size_t);
void* memmove(void* , void* , size_t);
void* memset(void*, uint8_t, size_t);
void* memcpy(void*, const void*, size_t);
#endif

#endif //  STRING_H