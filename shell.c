#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "shell.h"
#include <errno.h>
/*
What it does: forks a child process and executes the command, parent waits for child to exit
Arguments:
          command: null terminated array of pointers to strings
Return Value: none
*/
void run(char *command[]){
  int f = fork();
  int status;
  if(f == 0){
    execvp(command[0],command);
  }
  else wait(&status);
}
/*
What it does: gets the user and current directory and prints them to prompt user for command
Arguments: none
Return Value: none
*/
void promptUser(){
  char cwd[1024];
  cwd[1023]=0;
  getcwd(cwd,sizeof(cwd));
  char *user=getenv("USER");
  printf("%s %s$ ",cwd,user);
  printf("\nEnter Command:");
}

/*
What it does: searches in command for target and returns the index
Arguments: command: an array of pointers to strings
           target: a string
Return Value: the index in command where target is found
*/
int indexInArray(char *command[],char *target){
  int i=0;
  while(command[i]){
    if(strcmp(command[i],target)==0)return i;
    else i++;
  }
  return -1;
}

/*
What it does: Creates a pipe(redirect stdin of first command to stdout of other)between two commands and executes the second command
Arguments: pipecommand: an array of pointers to strings, where the
           pointers to the left of the pipe character(|) represent the command who's stdin is being redirected
           and the pointers to the right of | represent the command that will be executed

           pipelocation: index where pipe character(|) is found
Return Value: none
*/
void thepipe(char *pipecommand[],int pipelocation){
  int stdinDupe= dup(STDIN_FILENO);
  int stdoutDupe = dup(STDOUT_FILENO);
  char *tempfile = "pipefile.tmp";
  //stores the command we're piping to along with extra arguments
  char **pipeto = &pipecommand[pipelocation+1];
  //terminate at pipelocation so we're only left with the first command
  pipecommand[pipelocation]=0;
  //redirects stdout
  int fd=open(tempfile,O_CREAT|O_RDWR,0644);
  dup2(fd,STDOUT_FILENO);
  run(pipecommand);//run command one (gets redirected to file)
  dup2(stdoutDupe,STDOUT_FILENO);
  lseek(fd,0,SEEK_SET);
  //redirects stdin
  dup2(fd,STDIN_FILENO);
  run(pipeto);//run command 2(reads from file)
  dup2(stdinDupe,STDIN_FILENO);
  close(fd);
  remove(tempfile);
}


/*
What it does: Removes all white spaces in the given array of pointers
Arguments:
        command: array of pointers to string
Return Value: None
*/
void noMoreSpaces(char *command[]){
    char *temp[300];
    int i=0;
    int j=0;
    while(command[i]!=NULL){ //store all non white space characters in temp
      if(strcmp(command[i]," ")>0){
        temp[j]=command[i];
        j++;
    }
      i++;
    }
    i=0;
    while(i!=j){  //loops through command and sets corresponding index to the value in temp
      command[i]=temp[i];
      i++;
    }
    command[i]=0; //null terminate

}
/*
  What it does: changes directories
  Arguments:
    whereto: a pointer to the path
  Return Value: None
*/
void cd(char *whereto){
  if(chdir(whereto)==-1){
    printf("%s\n",strerror(errno));
  }
}
void redirOut(char *command[],int outLoc){
  char *file = command[outLoc+1]; //the file
  char **from = &command[0];//the command
  command[outLoc]=0;//terminate so we only have the command
  int stdoutDupe = dup(STDOUT_FILENO);
  int fd = open(file,O_RDWR|O_CREAT,0644);
  dup2(fd,STDOUT_FILENO); //redirect stdout
  run(from);
  dup2(stdoutDupe,STDOUT_FILENO); 
  close(fd);
}
void redirIn(char *command[],int inLoc){
  char *file = command[inLoc+1];
  char **to = &command[0];
  command[inLoc]=0;
  int stdinDupe = dup(STDIN_FILENO);
  int fd = open(file,O_RDWR|O_CREAT,0644);
  dup2(fd,STDIN_FILENO);
  run(to);
  dup2(stdinDupe,STDIN_FILENO);
  close(fd);
}
/*
What it does: prompts user for input, separates the input by semicolons, loops through the array of pointers created,
              separates the strings pointed to using whitespaces then runs the command by
              determining if cd, exit or pipe are called and executes those separately, otherwise calls the run function on the commands
Arguments: None
Return Value: None

Example User Inputs:
  Multiple commands: ls -a -l;pwd;echo hello;exit
  Piping: ls -a -l | wc
*/
void parse_run(){
    while(1){
      promptUser();
       char input[1024];
        fgets(input,sizeof(input),stdin);
        if(strstr(input,"\n"))input[strlen(input)-1]=0; //remove newline from fgets if it exists
        char *commands[sizeof(input)];
        char *p = input;
        int i=0;
        //separate the multiple commands by "; and add to commands array
        while(p!=NULL){
          commands[i]=strsep(&p,";");
          i++;
        }
        commands[i]=0; //null terminate so the while loop works without segfaulting
        noMoreSpaces(commands);
        int j=0;
        while(commands[j]){//goes through commands array and separates each command by " " then runs it
        char *line = commands[j];
            char *command[sizeof(line)];
            char *a = line;
            int k=0;
            //separates each command by " "
            while(a!=NULL){
                command[k]=strsep(&a," ");
                k++;
            }
            command[k]=0;  //null terminate each command
            noMoreSpaces(command);
            int pipelocation = indexInArray(command,"|");
            int inLoc = indexInArray(command,"<");
            int outLoc = indexInArray(command,">");
            if(strcmp(command[0],"cd")==0)cd(command[1]); //change directory
            else if(strcmp(command[0],"exit")==0)exit(0); //exit
            else if(pipelocation!=-1)thepipe(command,pipelocation); //pipe
            else if(inLoc!=-1)redirIn(command,inLoc);
            else if(outLoc!=-1)redirOut(command,outLoc);
            else run(command); //everything else
     j++;
    }
  }
}

int main() {
    parse_run();
    return 0;
}
