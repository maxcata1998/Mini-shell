#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define BUFFER_SIZE 80
#define MAX_COMMANDS 1000

typedef void (*fun_ptr) (char* args[]);
char inputCommands[MAX_COMMANDS][BUFFER_SIZE];
char* commands[] = {"exit", "help", "cd", "history"};
int size= 0;

void sigint_handler(int sig){
  write(1, "mini-shell terminated\n", 35);
  exit(0);
}

void cd(char* args[]){
  if(args[1]==NULL){
    printf("NULL\n");
    return;
  }
  if(chdir(args[1]) != 0) {
    printf("No such directory!\n");
  }
  return;
}

void help(char* args[]){
  printf("Please choose a command:\ncd\nhelp\nexit\nhistory\n");
}

void exitShell(char* args[]){
  exit(1);
}

void history(char* args[]){
  for(int i = 0; i<size; i++){
    printf("%s\n", inputCommands[i]);
  }
}

void otherCommand(char* args[]){
  if(fork()==0){
    printf("Command not found--Did you mean something else?\n");
    exit(1);
  }else{
    wait(NULL);
  }
  return;
}

void execution(char* args[], char* commands[], fun_ptr functions[]){
  int index = 0;
  int tag  = 0;
  while(commands[index]!=NULL && index<4){
    if(strcmp(args[0], commands[index]) == 0){
      functions[index](args);
      tag = 1;
      break;
    }
    index++;
  }
  if(tag==0) {
    otherCommand(args);
    return;
  }
}

int main(int argc, char** argv){
  signal(SIGINT, sigint_handler);
  fun_ptr functions[4]={&exitShell, &help, &cd, &history};
  char inputs[BUFFER_SIZE];
  while(1){
    printf("mini-shell> ");
    fgets(inputs, BUFFER_SIZE, stdin);
    char* split = strtok(inputs, " \t\n");
    char* args[BUFFER_SIZE];
    int i = 0;
    while(split!=NULL){
      args[i++] = split;
      split = strtok(NULL, " \t\n");
    }
    args[i] = NULL;
    strcpy(inputCommands[size++], args[0]);
    size = size % MAX_COMMANDS;
    execution(args, commands, functions);
  }       
  return 0;
}

