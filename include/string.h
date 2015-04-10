#ifndef _STRING_H
#define _STRING_H

char *strcpy(char *d, const char *s);

char *strncpy(char *dest, char *src,size_t n);

char *strcat(char *destin, char *source);

int strcmp(const char *s1,const char * s2);

int strncmp(const char *str1, const char *str2, size_t maxlen);

char *strstr(char *str1, char *str2);

size_t strlen(const char *s);

void *memset(void *s, int ch, size_t n);

#endif
