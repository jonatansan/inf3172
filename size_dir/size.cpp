#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

bool calSubDir(string& dir, off_t& nbOctet, int& nbFile){

	DIR* dirToRead;
	struct dirent* dirEntToOpen;
	
	dirToRead = opendir(dir.c_str()); 
	
	if(dirToRead == NULL){
		
		cout<<"Répertoire introuvable"<<endl;
		return false ;
	}
	
	chdir(dir.c_str()); //Go to the right dir

	while( (dirEntToOpen = readdir(dirToRead)) ){
			
		if( !(strcmp(dirEntToOpen->d_name, ".") == 0 ||
			strcmp(dirEntToOpen->d_name, "..") == 0)){
				
			if( dirEntToOpen->d_type == DT_DIR ){
		
				struct stat fileStat;

				if( stat(dirEntToOpen->d_name, &fileStat) ){
					printf("%s: %s\n", dirEntToOpen->d_name, strerror(errno));
					continue;
				}
		
				nbOctet += fileStat.st_size; 
				nbFile++;
		
				string sTemp(dirEntToOpen->d_name); 
				calSubDir(sTemp, nbOctet, nbFile); 
				chdir(".."); //return to the right dir
				
			} else {
			
				struct stat fileStat;
			
				if( stat(dirEntToOpen->d_name, &fileStat) ){
					printf("%s: %s\n", dirEntToOpen->d_name, strerror(errno));
					continue;
				}

				nbOctet += fileStat.st_size; 
				nbFile++;	
			}
		}
		
	}
	
	closedir(dirToRead); 
	return true; 
} 

int main(int argc, char** argv){

	off_t nbOctet = 0; 	
	int nbFile = 0;
	
	bool flag = true; 

	if(argc <= 1){
	
		cout<<"Répertoire introuvable"<<endl;
		return 0;
	}
	
	char realPath[1024];
	realpath(argv[1], realPath);
	
	string str(realPath);
	
	if(flag){
		flag = calSubDir(str, nbOctet, nbFile);
	} 
	
	if(flag){
		cout<<"Le répertoire "<<realPath<<" contient "<<nbFile<<" fichiers pour un total de "<<nbOctet<<" octets"<<endl;
	}
	//closedir(dirToRead); 
	return 0;
}
