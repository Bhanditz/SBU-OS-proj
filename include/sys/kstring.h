#include <sys/defs.h>

#ifndef _STRING_H
#define _STRING_H

char *kstrcpy(char *d, const char *s);

char *kstrncpy(char *dest, char *src,uint64_t n);

char *kstrcat(char *destin, char *source);

int kstrcmp(const char *s1,const char * s2);

int kstrncmp(const char *str1, const char *str2, uint64_t maxlen);

char *kstrstr(char *str1, char *str2);

uint64_t kstrlen(const char *s);

uint64_t kpow(uint64_t base, uint64_t times);

uint64_t katoi(char* str);

uint64_t k_oct_to_dec(uint64_t oct);

#endif
