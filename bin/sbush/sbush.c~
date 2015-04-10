//------------------------------------------
//Copyright © 2013 by Jun Zeng && Junao Wang 
//-----------All rights reserved------------
//------------------------------------------


#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "sbush.h"

int isContain(char *, char); 
char* trim(char *, char *);
void getSpeEnvp(char **);
int pwd(char *);
void prompt();
void parseCommand(char *, char *, char **);
int runScriptFile(char *, char **);
void dealWithCommand(char **, char **);
void dealWithPipe(int, char **, char **, char **);



int main(int argc, char* argv[], char* envp[]) {
	int i, num;
	char inputLine[MAX_LINE];
	char tmp[NAME_MAX];
	char* contents[10];
	char* commandGroup[10];

	//get the needed environment parameters
	getSpeEnvp(envp);

	//deal with the script.sh--------------------------------------------
	if(argc == 2) {
		runScriptFile(argv[1], envp);
		exit(0);
	}
	//-------------------------------------------------------------------

	printf("--------------------------------------------------------------\n");
	printf("--------------Welcome! Thanks for using SBUSH!----------------\n");
	printf("----------Copyright © 2015 by Jun Zeng && Junao Wang----------\n");
	printf("--------------------All rights reserved-----------------------\n");
	printf("--------------------------------------------------------------\n");
	printf("\n");

	
	while(1) {
		//the enter prifex title
		prompt();

		memset(inputLine, 0, sizeof(inputLine));
		memset(contents, 0, sizeof(contents));
		
		scanf("%[^\n]%*c", inputLine);

		trim(tmp, inputLine);

		//deal with the potential pipeline
		num = 0;
		if(!isContain(tmp, '|')) {
			//count the number of '|'
			for(i=0; i<strlen(tmp); i++) {
				if(tmp[i] == '|') {
					num++;
				}
			}
			//having num of '|' means having num+1 commands
			num++;

			parseCommand(tmp, "|", commandGroup);

			dealWithPipe(num, commandGroup, argv, envp);

		} else {

			parseCommand(tmp, " ", contents);
			
			dealWithCommand(contents, envp);    
		}
	}


	printf("\n\n");
}






