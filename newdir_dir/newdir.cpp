#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstring>

using namespace std;

int main(int argc, char** argv){
	
	struct stat st;
	
	if( stat(argv[1], &st) == -1 ){
		mkdir(argv[1],0700); 
		cout<<"Fichier créé"<<endl;
	} else {
		cout<<"Impossible de créer le dossier"<<endl; 
	}
	return 0;
}
