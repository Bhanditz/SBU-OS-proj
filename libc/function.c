#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sbush.h>


int isContain(char* str, char ch) {
	int i;
	for(i=0; i<strlen(str); i++) {
		if(str[i] == ch) {
			return 0;
		}
	}
	return -1;
}

char* trim(char *newStr, char *oldStr) {
	int i, index, len;
	index = 0;
	for(i=0; i<strlen(oldStr); i++) {
		if(oldStr[i] == ' ') {
			index++;
		} else {
			break;
		}
	}
	strcpy(newStr, oldStr+index);
	len = strlen(newStr)-1;
	for(i=len; i>=0; i--) {
		if(*(newStr+i) == ' ') {
			*(newStr+i) = '\0';
		} else {
			break;
		}
	}
	return newStr;
}

int readline(int fd, char* buf, size_t maxlen) {
	char tmp;
	int i = 0;

	while(1) {
		if(read(fd, &tmp, 1) == 0) {
			break;
		}
		if(tmp == '\n') {
			i++;
			break;
		}
		buf[i] = tmp;
		i++;
	}
	buf[i] = '\0';
	return i;
}

int getEnvp(char* envp[], char* name, char* parameter) {
	int i = 0;
	while(*envp[i]) {
		if(!strncmp(envp[i], name, strlen(name))) {
			strcpy(parameter, envp[i]+strlen(name)+1);
			break;
		}
		i++;
	}
	if(strlen(parameter) > 0) {
		return 0;
	} else {
		return -1;
	}
}

void getSpeEnvp(char* envp[]) {
	//deal with the USER envp
	if(getEnvp(envp, "USER", USER) < 0) {
		printf("[ERROR]: get USER envp failed\n");
	}
	//deal with the HOME envp
	if(getEnvp(envp, "HOME", HOME) < 0) {
		printf("[ERROR]: get HOME envp failed\n");
	}
	//deal with the PATH envp
	if(getEnvp(envp, "PATH", PATH) < 0) {
		printf("[ERROR]: get PATH envp failed\n");
	}
	strcpy(PS1, USER);
	strcat(PS1, "@SBU-SH");
}

int pwd(char *path) {
	char *pathName;
	
	if((pathName = malloc(NAME_MAX)) == NULL) {
		printf("[ERROR]: malloc error \n");
		return -1;
	}
	if(getcwd(pathName, NAME_MAX) < 0) {
		printf("[ERROR]: getcwd error \n");
		return -1;
	}
	if(strcpy(path, pathName) == NULL) {
		printf("[ERROR]: strcpy error \n");
		return -1;
	}
	free(pathName);
	return 0;
}

void parseCommand(char* inputLine, char* separator, char* contents[]) {
	 int i = 0;
	 char* buf = strstr(inputLine, separator);
	 while(buf != NULL) {
	 	buf[0] = '\0';
	 	contents[i] = malloc(NAME_MAX);
	 	strcpy(contents[i], inputLine);
	 	inputLine = buf + strlen(separator);
	 	buf = strstr(inputLine, separator);
	 	i++;
	 }
	 contents[i] = malloc(NAME_MAX);
	 strcpy(contents[i], inputLine);
}

void prompt() {
	char* pathName;
	pathName = malloc(MAX_LINE);
	if(pwd(pathName) < 0) {
		printf("[ERROR]: pwd error\n");
		exit(0);
	}
	if(!strncmp(pathName, HOME, strlen(HOME))) {
		printf("%s:~%s$ ", PS1, pathName+strlen(HOME));
	} else {
		printf("%s:%s$ ", PS1, pathName);
	}
	free(pathName);
}

void pwd_buildin() {
	char* pathName;
	pathName = malloc(MAX_LINE);
	if(pwd(pathName) < 0) {
		printf("[ERROR]: pwd error\n");
		exit(0);
	} else {
		printf("%s\n", pathName);
	}
	free(pathName);
}

int cd_buildin(char* parameter) {
	int i;
	char* pathName;
	pathName = malloc(MAX_LINE);
	if(pwd(pathName) < 0) {
		printf("[ERROR]: pwd error\n");
		exit(0);
	}
	if(!strcmp(parameter, "..")) {
		for(i=strlen(pathName)-1; i>=0; i--) {
			if(i == 0) {
				strcpy(pathName, "/");
				break;
			}
			if(pathName[i] == '/') {
				pathName[i] = '\0';
				break;
			}
		}
	} else if(!strncmp(parameter, "/", 1) || !strncmp(parameter, "~", 1)) {
		if(!strncmp(parameter, "~", 1)) {
			strcpy(pathName, HOME);
			strcat(pathName, parameter+1);
		} else {	
			strcpy(pathName, parameter);
		}
	} else {
		strcat(pathName, "/");
		strcat(pathName, parameter);
	}
	if(chdir(pathName) < 0) {
		printf("[ERROR]: sbush: cd: %s: No such file or directory\n", parameter);
		return -1;
	}
	free(pathName);
	return 0;
}

int export_buildin(char* parameter[]) {
	
	int i;
	char str[MAX_LINE];
	char* contents[2];
	i = 1;
	memset(str, 0, sizeof(str));
	memset(contents, 0, sizeof(contents));
	while(parameter[i] != NULL) {
		strcat(str, parameter[i]);
		i++;
	}
	parseCommand(str, "=", contents);
	if(!strcmp(contents[0], "PATH")) {
		memset(PATH, 0, sizeof(PATH));
		strcpy(PATH, contents[1]);
	} else if(!strcmp(contents[0], "HOME")) {
		memset(HOME, 0, sizeof(HOME));
		strcpy(HOME, contents[1]);
	} else if(!strcmp(contents[0], "PS1")) {
		memset(PS1, 0, sizeof(PS1));
		strcpy(PS1, contents[1]);
	} else {
		printf("[ERROR]: sbush: %s: Invalid environment parameter\n", contents[0]);
		return -1;
	}


	return 0;
}



int externalCommand(char* command, char* argv[], char* envp[]) {
	int i = 0;
	char dir[MAX_LINE]; 
	char* subPath[50];

	memset(subPath, 0, sizeof(subPath));
	parseCommand(PATH, ":", subPath);

	if(!strncmp(command, "/", 1)) {
		if(execve(command, argv, envp) < 0) {
			return -1;
		}
	}

	while(subPath[i] != NULL) {
		memset(dir, 0, sizeof(dir));
		strcpy(dir, subPath[i]);
		strcat(dir, "/");
		strcat(dir, command);
		execve(dir, argv, envp);
		i++;
	}
	return -1;
}

int executeFile(char* fileName, char* argv[], char* envp[]) {
	char* dir;
	dir = fileName + 2;
	if(!isContain(dir, '/')) {
		dir = fileName + 1;
	}
	if(execve(dir, argv, envp) < 0) {
		return -1;
	}
	return 0;
}

//deal with the running of script file 
int runScriptFile(char* command, char* envp[]) {
	int fd, pid, status, n;
	char buf[NAME_MAX];
	char* contents[10];
	char* fileName = command;

	if((fd = open(fileName, O_RDONLY)) < 0) {
		printf("[ERROR]: sbush: %s:  No such file or directory\n", fileName);
		return -1;
	}

	while(1) {
		memset(buf, 0, sizeof(buf));
		memset(contents, 0, sizeof(contents));
		n = readline(fd, buf, NAME_MAX);
		if(n == 0) {
			break;
		}
		parseCommand(buf, " ", contents);
		if((pid = fork()) == 0) {
			externalCommand(contents[0], contents, envp);
			exit(0);
		} else {
			waitpid(pid, &status, 0);
		}
	}
	close(fd);
	return 0;
}

//deal with all the distinct commands
void dealWithCommand(char* contents[], char* envp[]) {
	pid_t pid;
	int status;
	char tmp[NAME_MAX];

	trim(tmp, contents[0]);

	if(!strcmp(tmp, "cd")) {
		if(contents[1] == NULL) {
			chdir(HOME);
		} else {
			cd_buildin(contents[1]);
		} 
	} else if(!strcmp(tmp, "pwd")) {
		pwd_buildin();
	} else if(!strcmp(tmp, "export")) {
		memset(tmp, 0, sizeof(tmp));
		export_buildin(contents);

	}else if(!strncmp(tmp, "./", 2)) {
		//deal with the executable files
		if((pid = fork()) == 0) {
			if(executeFile(tmp, contents, envp) < 0) {
				printf("[ERROR]: sbush: %s: File cannot execute\n", contents[0]);
			}
			exit(0);
		} else {
			waitpid(pid, &status, 0);
		}
	} else {
		// deal with the binaries
		if((pid = fork()) == 0) {
			if(externalCommand(tmp, contents, envp) < 0) {
				printf("[ERROR]: sbush: %s: Command not found\n", contents[0]);
			}
			exit(0);
		} else {
			waitpid(pid, &status, 0);
		}
	}   
}

void closePfd(int num, int pfd[][2]) {
	int i, j;
	for(i=0; i<num; i++) {
		for(j=0; j<2; j++) {
			close(pfd[i][j]);
		}
	} 
}

//deal with the pipeline
void dealWithPipe(int num, char* commandGroup[], char* argv[], char* envp[]) {
	int i, status, pid[num];
	int pfd[num][2];
	char tmp[NAME_MAX];
	char* contents[10];

	memset(tmp, 0, sizeof(tmp));
	memset(contents, 0, sizeof(contents));

	for(i=0; i<num; i++) {
		pipe(pfd[i]);
	}

	//the head command
	if((pid[0] = fork()) == 0) {
    	dup2(pfd[0][1], 1);
	    closePfd(num, pfd);	    
    	trim(tmp, commandGroup[0]);
    	parseCommand(tmp, " ", contents);
    	externalCommand(contents[0], contents, envp);    	
    	exit(0);
  	}

  	//the body command
  	for(i=1; i<num-1; i++) {
	  	if ((pid[i] = fork()) == 0) {
	    	dup2(pfd[i-1][0], 0);
	    	dup2(pfd[i][1], 1);
		    closePfd(num, pfd);
	    	trim(tmp, commandGroup[i]);
    		parseCommand(tmp, " ", contents);
    		externalCommand(contents[0], contents, envp);
	    	exit(0);
	  	}
	}

  	//the tail command
  	if((pid[num-1] = fork()) == 0) {
    	dup2(pfd[num-2][0], 0);
    	closePfd(num, pfd);
    	trim(tmp, commandGroup[num-1]);
		parseCommand(tmp, " ", contents);
		externalCommand(contents[0], contents, envp);
    	exit(0);
  	}
  	//close all the pipe entries in parent process
  	closePfd(num, pfd);

  	for(i=0; i<num; i++) {
  		waitpid(pid[i], &status, 0);
  	}

}












