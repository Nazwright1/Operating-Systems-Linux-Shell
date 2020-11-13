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
// home directory
char home[FILENAME_MAX];
int numCommands = 0;
bool pathSet = false;
std::vector<char*>paths;
char* path;

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

void setPathVariable(char* newpath){
  path = newpath; 
}

bool getPathStatus(){
  return pathSet; 
}

void changePathStatus(bool status){
  pathSet = status;
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

std::vector<char*> parseCommands( char* commands ){
  std::vector<char*> arguments;
  char *prog = strtok( commands, " " );
  char *tmp = prog;
  while( tmp != NULL ) {
    arguments.push_back(tmp);
    tmp = strtok( NULL, " " );
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
  char* current_directory = strdup( ash );
  while( 1 ){
  	 setHomeDirectory();
     printPrompt( current_directory );
     // need to free this once done.
     char* commands = getCommands();
     
     if( strlen( commands ) != 0 ){
       std::vector<char*> commandsVector = parseCommands(commands);
       char** linuxCommands = vectToArray(commandsVector);

       if ( strcmp( linuxCommands[0], "exit" ) == 0 ){
         std::cout << "Nazere Wright, Shayla Sexton, Kevin Hilliard\n";
         return 0;
       }

       if ( strcmp( linuxCommands[0], "cd" ) == 0 ){
        // cd should onlu take one argument
         if( chdir( linuxCommands[1]) == 0 && numCommands == 2){
           std::cout << "change directory succesful\n";
           continue;
         }
         else{
          std::cout << "change directory error\n";
          continue;
         }

       }

       if( strcmp( linuxCommands[0], "path" ) == 0 ){
         changePathStatus(true);
         if(numCommands == 1){
          // we are not setting the path only running builtin
          // print prompt 
          continue;
         }
         else{
           for(int i = 1; i < numCommands; i++){
             paths.push_back(linuxCommands[i]);
           }
         }
       }
 

     }
     // the user just presses enter
     else{
       // print the prompt
       continue;
     }
   
  }
}