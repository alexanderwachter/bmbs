#include "string.h"
#include <stdint.h>
#include <stddef.h>
/*Mostly copied from linux lib/string.c*/

char *strcpy(char *dest, const char *src)
 {
  char *tmp = dest;

  while ((*dest++ = *src++) != '\0');
    return tmp;
 }

 char *strncpy(char *dest, const char *src, size_t count)
{
  char *tmp = dest;
  while (count--)
  {
    if ((*tmp = *src) != 0)
      src++;
      tmp++;
  }
  return dest;
}

int strcmp(const char *cs, const char *ct)
{
  unsigned char c1, c2;

  while (1) {
    c1 = *cs++;
    c2 = *ct++;
    if (c1 != c2)
      return c1 < c2 ? -1 : 1;
    if (!c1)
      break;
  }
  return 0;
}

int strncmp(const char *cs, const char *ct, size_t count)
{
  unsigned char c1, c2;

  while (count) {
    c1 = *cs++;
    c2 = *ct++;
    if (c1 != c2)
      return c1 < c2 ? -1 : 1;
    if (!c1)
      break;
    count--;
  }
  return 0;
}

size_t strlen(const char *s)
{
  const char *sc;

  for (sc = s; *sc != '\0'; ++sc);
  return sc - s;
}

void *memcpy(void *dest, const void *src, size_t count)
{
  char *tmp = dest;
  const char *s = src;

  while (count--)
    *tmp++ = *s++;
  return dest;
}

int memcmp(const void *cs, const void *ct, size_t count)
{
  const unsigned char *su1, *su2;
  int res = 0;

  for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
    if ((res = *su1 - *su2) != 0)
    break;
  return res;
}

void *memset(void *s, int c, size_t count)
{
	char *xs = s;

	while (count--)
		*xs++ = c;
	return s;
}
