#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int scanInt(char*, int, int*); 		//%d
int scanCh(char*, int, int*); 		//%c
int scanStr(char*, int, char*); 	//%s

int scanf(const char *format, ...) {

	va_list 	val;
	char 		recvBuffer[MAX_BUFF];
	int  		index = 0;
	int 		printed = 0;
	int* 		valInt;
	int* 		valCh;
	char* 		valStr = NULL;

	va_start(val, format);
	read(0, recvBuffer, MAX_BUFF);
	while(*format) {
		if(*format == '%') {
			format++;
			printed++;
			if(*format == 'd') {
				valInt = va_arg(val, int*);
				index = scanInt(recvBuffer, index, valInt);

			} else if(*format == 'c') {
				valCh = va_arg(val, int*);
				index = scanCh(recvBuffer, index, valCh);

			} else if(*format == 's') {
				valStr = va_arg(val, char*);
				index = scanStr(recvBuffer, index, valStr);
			}
		}
		format++;
	}

	va_end(val);

	return printed;
}

int scanInt(char* recvBuffer, int index, int* valInt){
	int tmp = 0;
	while(1){
		if(*(recvBuffer+index) >= '0' && *(recvBuffer+index) <= '9') {
			tmp *= 10;
			tmp += (*(recvBuffer+index) - '0');
			index++;
		} else {
			break;
		}
	}
	*valInt = tmp;
	return index;
}

int scanCh(char* recvBuffer, int index, int* valCh){
	*valCh = *(recvBuffer+index);
	index++;
	return index;
}

int scanStr(char* recvBuffer, int index, char* valStr){
	char* tmp = valStr;
	while(*(recvBuffer+index) != '\0' && *(recvBuffer+index) != '\n') {
		*tmp = *(recvBuffer+index);
		tmp++;
		index++;
	}
	return index;
}


