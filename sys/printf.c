#include <sys/sbunix.h>
#include <stdarg.h>

volatile int xAxis, yAxis;

//---------------clear------------------------------
void clear() {
	int x, y;
	register char* pos;

	pos = (char*)0xFFFFFFFF800B8000;
	for(y=0; y<25; y++) {
		for(x=0; x<160; x++)  {
			*(pos + x + 160*y) = 0;
		}
	}
	xAxis = 0;
	yAxis = 0;
}

//----------------printf----------------------------
void printInt(int); 		//%d
void printCh(char); 		//%c
void printStr(char*); 		//%s
void printHex(uint64_t); 	//%x
void printPtr(uint64_t); 	//%p
void scroll(void);			//scroll the screen

void sys_write(const char*, int, int);

void printf(const char *fmt, ...) {

	va_list 	val;
	int 		valInt = 0;
	char 		valCh = 0;
	char* 		valStr = NULL;
	uint64_t	valHex = 0;	
	uint64_t	valPtr = 0;

	va_start(val, fmt);

	while(*fmt) {
		if(*fmt == '%') {
			fmt++;
			if(*fmt == 'd') {
				valInt = va_arg(val, int);
				printInt(valInt);

			} else if(*fmt == 'c') {
				valCh = va_arg(val, int);
				printCh(valCh);

			} else if(*fmt == 's') {
				valStr = va_arg(val, char*);
				printStr(valStr);

			} else if(*fmt == 'x') {
				valHex = va_arg(val, uint64_t);
				printHex(valHex);

			} else if(*fmt == 'p') {
				valPtr = va_arg(val, uint64_t);
				printPtr(valPtr);
			}

		} else {
			sys_write(fmt, 1, 0x08);
		}
		fmt++;
	}
	va_end(val);
}

void sys_write(const char *fmt, int len, int color) {
	register char* 		pos;
	while(len && *fmt) {
		if(*fmt == '\r') {
			xAxis = 0;
			fmt++;
			len--;
			continue;
		}
		if(*fmt == '\n') {
			xAxis = 0;
			yAxis++;
			fmt++;
			len--;
			continue;
		}
		scroll();
		pos = (char*)0xFFFFFFFF800B8000 + xAxis + 160*yAxis;
		*pos = *fmt;
		*(pos+1) = color;
		xAxis += 2;
		if(xAxis >= 160) {
			xAxis = 0;
			yAxis++;
		}
		if(yAxis > 25) {

		}
		fmt++;
		len--;
	}
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
	sys_write(&argument, 1, 0x08);
}


void printStr(char* argument) {
	while(*argument) {
		printCh(*argument);
		argument++;
	}
}

void printHex(uint64_t argument) {
	int 		i = 0, prei = 0, dif = 0;
	uint64_t 	tmp = 0;
	uint64_t 	arg = argument;	

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

void printPtr(uint64_t argument) {
	printf("0x");
	printHex(argument);
}

void scroll() {
	int i, j;
	register char* 		pos;

	if(yAxis > 22) {
		for(i=0; i<23; i++) {
			for(j=0; j<160; j++) { 
				pos = (char*)0xFFFFFFFF800B8000 + j + 160*i;
				*pos = *(pos+160);
			}
		}
		for(j=0; j<80; j+=2) {
			pos = (char*)0xFFFFFFFF800B8000 + j + 160*23;
			*pos = 0;
		}
		yAxis--;
	}
}

