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

//char** parseCommands(char* commands){
  
 // char *prog = strtok(commands, " ");
 // char *tmp = prog;
 // while(tmp != NULL) {
    
   // tmp = strtok(NULL, " ");
  //}
//}




int main(){
  char* current_directory = strdup(ash);
  while(1){
  	 setHomeDirectory();
     printPrompt(current_directory);
     // need to free this once done.
     char* commands = getCommands();
     if(strlen(commands) != 0){
     char* newhome = getDirectoryname(home);
     std::cout << formatDirectory(newhome);
       
     }
     else{
       // print the prompt
       continue;
     }
   
  }
}