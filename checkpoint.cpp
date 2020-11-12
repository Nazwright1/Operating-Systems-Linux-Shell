#include <stdio.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <libgen.h>

// ash directory
const char* ash = "ash";
int numArguments = 0;
char home[FILENAME_MAX];
int numCommands = 0;


// print the current directory
void printPrompt(char* directory){
  if(directory){
    std::cout<< directory << " >";
  }
  else{
  	std::cout<< "print error";
  	exit(1);
  }
}

// takes a directory name and adds slashes
char* formatDirectory(char* directory){
  const char* slash = "/";
  char* temp = strdup(slash);
  strcat(temp, directory);
  strcat(temp, slash);
  return temp; 
}

// gets the dir name of the current directory
char* getDirectoryname(char* dir_path){
	// if path is /user/username/ then this returns username
  return basename(dir_path);
}

void setHomeDirectory(){
   getcwd(home, sizeof(home));
}

// get a char* of commands from the prompt
char* getCommands(){
  char* commandLine = (char*)malloc( FILENAME_MAX );
  std::cin.getline(commandLine, FILENAME_MAX);
  return commandLine;
}

char** vectToArray(std::vector<char*>argsVector){
  char** argv = new char *[argsVector.size() + 1];
  for (int k = 0; k < argsVector.size(); k++) {
    argv[k] = argsVector[k];
  }
  argv[argsVector.size()] = NULL;
  return argv;
}

std::vector<char*> parseCommands(char* commands){
  std::vector<char*> arguments;
  char *prog = strtok(commands, " ");
  char *tmp = prog;
  while(tmp != NULL) {
    arguments.push_back(tmp);
    tmp = strtok(NULL, " ");
  }

  if(!arguments.empty()) {
  	numCommands = arguments.size();
    return arguments;
  }
  else{
    return std::vector<char*>();
  }
}

int main(){
  char* current_directory = strdup(ash);
  while(1){
  	 setHomeDirectory();
     printPrompt(current_directory);
     // need to free this once done.
     char* commands = getCommands();
     if(strlen(commands) != 0){
       std::vector<char*> commandsVector = parseCommands(commands);
       char** linuxCommands = vectToArray(commandsVector);
     }
     else{
       // print the prompt
       continue;
     }
   
  }
}