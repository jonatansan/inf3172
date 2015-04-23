#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv){

	char currentdir[1024];
	if(getcwd(currentdir, sizeof(currentdir))){
		//On ne fait rien, on affiche le currentdir plus tard
	} else {
		printf("getcwd() error %d", errno);
	}

	DIR* dirToRead;
	struct dirent*  dirEntToOpen;
	
	dirToRead = opendir("."); 
	
	if(argc > 1 && strcmp(argv[1],"-d") == 0){	
	
		printf("Sous-répertoires de %s\n", currentdir); 
		
		while( (dirEntToOpen = readdir(dirToRead)) ){

			if( !(	strcmp(dirEntToOpen->d_name, ".") == 0 ||
					strcmp(dirEntToOpen->d_name, "..") == 0) &&
					dirEntToOpen->d_type == DT_DIR){
				
				cout<<dirEntToOpen->d_name<<endl;
			}
		}
	} else {
	
		printf("Fichiers de %s\n", currentdir); 
			
		while( (dirEntToOpen = readdir(dirToRead)) ){

			if( dirEntToOpen->d_type == DT_REG){
				
				cout<<dirEntToOpen->d_name<<endl;
			}
		}
	}
	
	closedir(dirToRead); 
	return 0;
}
