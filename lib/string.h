#ifndef _STRING_H_
#define _STRING_H_
#include <stdint.h>
#include <stddef.h>

char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t count);
int strcmp(const char *cs, const char *ct);
int strncmp(const char *cs, const char *ct, size_t count);
size_t strlen(const char *s);
void* memcpy (void* dstpp, const void *srcpp, size_t len);
int memcmp(const void *cs, const void *ct, size_t count);
void *memset(void *s, int c, size_t count);

#endif
