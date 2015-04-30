#include <sys/kstring.h>
#include <sys/defs.h>



char* kstrcpy(char *d, const char *s) {
    char *tmp = d;
    while(*s) {
        *tmp = *s;
        tmp++;
        s++;
    }
    *tmp = '\0';
    return d;
}

char* kstrncpy(char *dest, char *src, uint64_t n) {
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

int kstrcmp(const char* s1, const char* s2) {
    while((*s1 == *s2)) {
        s1++;
        s2++;
        if(*s1 == '\0' && *s2 == '\0') {
            break;
        }
    }
    return *s1-*s2;
}


int kstrncmp(const char *str1, const char *str2, uint64_t maxlen) {
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

char* kstrcat(char* destin, char* source) {
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

char* kstrstr(char* str1, char* str2) {
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

uint64_t kstrlen(const char *s) {
    uint64_t len = 0;
    while(*s) {
        len++;
        s++;
    }
    return len;
}

uint64_t kpow(uint64_t base, uint64_t times) {
    uint64_t ret = 1;
    while(times) {
        ret *= base;
        times--;
    }
    return ret;
}

uint64_t katoi(char* str) {
    uint64_t ret = 0;
    while(*str) {
        ret *= 10;
        ret += (*str - '0');
        str++;
    }
    return ret;
}

uint64_t k_oct_to_dec(uint64_t oct) {
    uint64_t ret = 0, tmp = 0, power = 0;
    while(oct) {
        tmp = oct%10;
        oct /= 10;
        ret += (tmp * kpow(8, power));
        power++;
    }
    return ret;
}



