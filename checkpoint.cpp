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
char* directory;

// print the current directory
void printPrompt(){
    std::cout<< directory << " >";
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

//trigger if the path was set or not
void changePathStatus(bool status){
  pathSet = status;
}

// gets the dir name of the current directory
char* getDirectoryname(char* dir_path){
	// if path is /user/username/ then this returns username
  return basename(dir_path);
}

// set the home directory
void setHomeDirectory(){
   getcwd(home, sizeof(home));
}

void changeDirectory(char* newdir){
  if( strcmp( directory, ash ) == 0 ){
    char* temp = (char*) malloc( FILENAME_MAX );
    strcat( temp, ash );
    strcat( temp, newdir );
    strcat( temp, "/" );
    strcpy( directory, temp );
  }
}

// get a char* of commands from the prompt
char* getCommands(){
  char* commandLine = (char*)malloc( FILENAME_MAX );
  std::cin.getline(commandLine, FILENAME_MAX);
  return commandLine;
}

// convert a vector to an array of char pointers
char** vectToArray(std::vector<char*>argsVector){
  char** argv = new char *[argsVector.size() + 1];
  for (int k = 0; k < argsVector.size(); k++) {
    argv[k] = argsVector[k];
  }
  argv[argsVector.size()] = NULL;
  return argv;
}

//parse user input into a vector

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

//get the existing executable search path from user-specified paths
char* getExistingPath( std::vector<char*>searchPaths ){
  char* existingpath;
  for( int i = 0; i < searchPaths.size(); i++ ){
    if( access(searchPaths.at(i) , X_OK ) == 0 ){
      existingpath = strdup( searchPaths.at(i) );
      changeDirectory( dirname(searchPaths.at(i)) );
      break;
    }
  }
  return existingpath;
}


// checks if this needle (value) is in the vector
bool Exists( char* needle, std::vector<char*> haystack ){
  bool returnval;
  for( int i = 0; i < haystack.size(); i++ ){
    if( strcmp( needle, haystack.at(i) ) == 0 ){
      returnval = true;
      break;
    }
    else{
      if( i == haystack.size() - 1 ){
        returnval = false;
      }
    }
  }
  return returnval;
}

int main(){
  directory = strdup( ash );
  while( 1 ){
  	 setHomeDirectory();
     printPrompt();
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
         if( chdir( linuxCommands[1]) == 0 && numCommands == 2 ){

           std::cout << "change directory succesful\n";
           continue;
         }
         else{
          std::cout << "change directory error\n";
          continue;
         }

       }

       if( strcmp( linuxCommands[0], "path" ) == 0 ){
         changePathStatus( true );
         if( numCommands == 1 ){
          // we are not setting the path only running builtin
          // print prompt 
          continue;
         }
         else{
          //
           for( int i = 1; i < numCommands; i++ ){
             if( Exists( linuxCommands[i], paths ) ){
              continue;
             }
             else{
               paths.push_back( linuxCommands[i] );
             }
           }
         }
       }
       else{
        pid_t kidpid = fork();
        if( kidpid < 0 ){
          perror( "Could not fork" );
        }
        // child process
        else if( kidpid == 0 ){
          // if the path has been set
          if( getPathStatus() ){
            // full executable search paths
            std::vector<char*> executables;
            for( int i = 0; i < paths.size(); i++ ){

              // make enough space for the path combined with the command 
              // +1 for the null terminated byte string at the end.
              char* temp = (char*) malloc( strlen( paths.at(i) ) + strlen( linuxCommands[0] ) + 1);
              strcat( temp, paths.at(i) );
              strcat( temp, linuxCommands[0] );
              executables.push_back(temp);
            }
            // the existing search path out of all the possibilities
            strcpy( linuxCommands[0] , getExistingPath( executables ) );

            //std::cout << dirname(linuxCommands[0]);
              
              //pipe the directory through to the parent and let them print it
            
             execv( linuxCommands[0], linuxCommands );
            //if( execv( linuxCommands[0], linuxCommands ) == -1 ){
              //std::cout << "Error execv";
           //}
          }
          else{
            std::cout << "could not find the command: ";
          }
        }

        else if( kidpid > 0 ){
          if( waitpid( kidpid , 0, 0 ) < 0 ){
            return -1;
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