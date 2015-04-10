#define MAX_LINE 2048
#define BUF_SIZE 1024
//GLOBAL VAL
//environment variable
char USER[NAME_MAX];
char HOME[MAX_LINE];
char PATH[MAX_LINE];
char PS1[MAX_LINE];

//use for readdir
int nread;
char buf[BUF_SIZE];

//functions
int isContain(char* str, char ch);
char* trim(char *newStr, char *oldStr);
int readline(int fd, char* buf, size_t maxlen);
int getEnvp(char* envp[], char* name, char* parameter);
void getSpeEnvp(char* envp[]);
int pwd(char *path);
void parseCommand(char* inputLine, char* separator, char* contents[]);
void prompt();
void pwd_buildin();
int cd_buildin(char* parameter);
int export_buildin(char* parameter[]);
int externalCommand(char* command, char* argv[], char* envp[]);
int executeFile(char* fileName, char* argv[], char* envp[]);
int runScriptFile(char* command, char* envp[]);
void dealWithCommand(char* contents[], char* envp[]);
void closePfd(int num, int pfd[][2]);
void dealWithPipe(int num, char* commandGroup[], char* argv[], char* envp[]);
