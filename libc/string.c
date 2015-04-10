#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/defs.h>



char* strcpy(char *d, const char *s) {
    char *tmp = d;
    while(*s) {
        *tmp = *s;
        tmp++;
        s++;
    }
    *tmp = '\0';
    return d;
}

char* strncpy(char *dest, char *src, size_t n) {
    char *tmp = dest;
    while(n > 0 && *src) {
        *tmp = *src;
        tmp++;
        src++;
        n--;
    }
    *tmp = '\0';
    return dest;
}

int strcmp(const char* s1, const char* s2) {
    while((*s1 == *s2)) {
        s1++;
        s2++;
        if(*s1 == '\0' && *s2 == '\0') {
            break;
        }
    }
    return *s1-*s2;
}


int strncmp(const char *str1, const char *str2, size_t maxlen) {
    while(maxlen--) {
        if(*str1 != *str2) {
            return *str1 - *str2;
        } else if(*str1 == '\0' || *str2 == '\0') {
            return 0;
        }else {
            str1++;
            str2++;
        }
    }
    return 0;
}

char* strcat(char* destin, char* source) {
    char* tmp=destin;
    while(*destin) {
        destin++;
    }
    while(*source) {
        *destin = *source;
        destin++;
        source++;
    }
    //*tmp = '\0';
    return tmp;
}

char* strstr(char* str1, char* str2) {
    char* string;
    char* search;
    char* tmp;
    int offset = 0;
    if(!str1 || !str2) {
        return NULL;
    }
    string = str1;
    while(*string) {
        search = str2;
        while(*string != *search && *string) {
            string++;
            offset++;
        }
        tmp = string;
        while(*tmp == *search) {
            tmp++;
            search++;
            if(*search == '\0') {
                return str1+offset;
            }
        }
        string++;
        offset++;
    }
    return NULL;
}

size_t strlen(const char *s) {
    size_t len = 0;
    while(*s) {
        len++;
        s++;
    }
    return len;
}

void *memset(void *s, int ch, size_t n) {
    char* tmp = s;
    while(n > 0) {
        *tmp = ch;
        tmp++;
        n--;
    }
    return s;
}



