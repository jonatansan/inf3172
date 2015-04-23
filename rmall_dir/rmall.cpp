#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

using namespace std;

void checkPermission(string& dir, bool& permissionDenied, bool& repNotFound){

	if(permissionDenied) return;  // 
	
	string tempName = "tempName";  
	
	DIR* dirToRead;
	struct dirent* dirEntToOpen;
	
	dirToRead = opendir(dir.c_str()); 
	
	if(dirToRead == NULL){
		cout<<"Répertoire introuvable"<<endl;
		repNotFound = true; 
		return ;
	}
	
	chdir(dir.c_str()); //Go to the right dir

	while( (dirEntToOpen = readdir(dirToRead)) ){
			
		if( !(strcmp(dirEntToOpen->d_name, ".") == 0 ||
			strcmp(dirEntToOpen->d_name, "..") == 0)){
			
			string oldName(dirEntToOpen->d_name);
			
			if( dirEntToOpen->d_type == DT_DIR ){
		
					
				string sTemp(dirEntToOpen->d_name);
				checkPermission(sTemp, permissionDenied, repNotFound); 
				chdir(".."); //return to the right dir
				
				if(rename(dirEntToOpen->d_name, tempName.c_str()) != 0){		
					permissionDenied = true; 
					break;
				} else {
					rename(tempName.c_str(), oldName.c_str()); 
				}	
			} else {
			
				if(rename(dirEntToOpen->d_name, tempName.c_str()) != 0){		
					permissionDenied = true; 
					break;
				} else {
					rename(tempName.c_str(), oldName.c_str()); 
				}	
			}
		}
		
	}
	
	closedir(dirToRead); 
} 

void delSubDir(string& dir, bool& repNotFound){
	
	DIR* dirToRead;
	struct dirent* dirEntToOpen;
	
	dirToRead = opendir(dir.c_str()); 
	
	if(dirToRead == NULL){
		cout<<"Répertoire introuvable"<<endl;
		repNotFound = true; 
		return ;
	}
	
	chdir(dir.c_str()); //Go to the right dir

	while( (dirEntToOpen = readdir(dirToRead)) ){
			
		if( !(strcmp(dirEntToOpen->d_name, ".") == 0 ||
			strcmp(dirEntToOpen->d_name, "..") == 0)){
				
			if( dirEntToOpen->d_type == DT_DIR ){
		
				string sTemp(dirEntToOpen->d_name);
				delSubDir(sTemp, repNotFound); 
				chdir(".."); //return to the right dir

				if(rmdir(dirEntToOpen->d_name) != 0){
					printf("%s: %s\n", dirEntToOpen->d_name, strerror(errno));
					continue;		
				}				
			} else {
							
				if( remove(dirEntToOpen->d_name) != 0 ){
					printf("%s: %s\n", dirEntToOpen->d_name, strerror(errno));
					continue;
				}
			}
		}
		
	}
	
	closedir(dirToRead); 
} 

int main(int argc, char** argv){

	bool permissionDenied = false; 
	bool repNotFound = false; 

	if(argc <= 1){
	
		cout<<"Répertoire introuvable"<<endl;
		return 0; 
	}
	
	char realPath[1024];
	realpath(argv[1], realPath);
	
	string str(realPath);
	
	checkPermission(str, permissionDenied, repNotFound); 
	if(!permissionDenied && !repNotFound){
		delSubDir(str,repNotFound);
		rmdir(realPath); 
	} 
	
	if(repNotFound){
	 	cout<<"Répertoire introuvable"<<endl;
	}
	
	else if(permissionDenied){
	 	cout<<"Permissions insuffisantes"<<endl;
	}
	
	else{
	 	cout<<"Répertoire supprimé"<<endl;
	}
	//closedir(dirToRead); 
	return 0;
}
