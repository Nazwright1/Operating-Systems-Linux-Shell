#include <stdio.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

// ash directory
const char* ash = "ash";
int numArguments = 0;

void printPrompt(char* directory){
  if(directory){
    std::cout<< directory << " >";
  }
  else{
  	std::cout<< "print error";
  	exit(1);
  }
}

char* getCommands(){
  char* commandLine = (char*)malloc( FILENAME_MAX );
  std::cin.getline(commandLine, FILENAME_MAX);
  return commandLine;
}

//char** parseCommands(char* commands){
  
//}




int main(){
  char* current_directory = strdup(ash);
  while(1){
     printPrompt(current_directory);
     char* commands = getCommands();
     if(strlen(commands) != 0){
       std::cout <<  commands << "\n";
     }
     else{
       continue;
     }
   
  }
}