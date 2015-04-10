#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void printInt(int); 	//%d
void printCh(char); 	//%c
void printStr(char*); 	//%s
void printHex(int); 	//%x
//void printPtr(int); 	//%p

int printf(const char *format, ...) {
	va_list 	val;
	int 		printed = 0;
	int 		valInt = 0;
	char 		valCh = 0;
	char* 		valStr = NULL;

	va_start(val, format);

	while(*format) {
		if(*format == '%') {
			format++;
			if(*format == 'd') {
				valInt = va_arg(val, int);
				printInt(valInt);

			} else if(*format == 'c') {
				valCh = va_arg(val, int);
				printCh(valCh);

			} else if(*format == 's') {
				valStr = va_arg(val, char*);
				printStr(valStr);

			} else if(*format == 'x') {
				valInt = va_arg(val, int);
				printHex(valInt);
			}

		} else {
			write(1, format, 1);
		}
		printed++;
		format++;
	}

	va_end(val);

	return printed;
}

void printInt(int argument) {
	int tmp = 0, i = 0, prei = 0, dif = 0;
	int arg = argument;	
	if(argument < 10) {
		printCh(argument + '0');
		return;
	}
	while(1) {
		if(arg < 10) {
			break;
		}
		while(i > 0) {
			tmp *= 10;
			i--;
		}
		arg -= tmp;
		tmp = arg;
		i = 0;
		while(tmp >= 10) {
			tmp /= 10;
			i++;
		}
		dif = prei - i;
		while(dif > 1) {
			printCh('0');	
			dif--;
		}
		printCh(tmp + '0');
		prei = i;
	}
}

void printCh(char argument) {
	write(1, &argument, 1);
}


void printStr(char* argument) {
	while(*argument != '\0') {
		printCh(*argument);
		argument++;
	}
}

void printHex(int argument) {
	int tmp = 0, i = 0, prei = 0, dif = 0;
	int arg = argument;	
	printStr("0x");
	if(argument < 16) {
		if(argument < 10) {
			printCh((char)(argument + '0'));		
		} else {
			printCh((char)(argument - 10 + 'a'));
		}
		return;
	}
	while(1) {
		if(arg < 16) {
			break;
		}
		while(i > 0) {
			tmp *= 16;
			i--;
		}
		arg -= tmp;
		tmp = arg;
		i = 0;
		while(tmp >= 16) {
			tmp /= 16;
			i++;
		}
		dif = prei - i;
		while(dif > 1) {
			printCh('0');	
			dif--;
		}
		if(tmp < 10) {
			printCh((char)(tmp + '0'));		
		} else {
			printCh((char)(tmp - 10 + 'a'));
		}
		prei = i;
	}
}

