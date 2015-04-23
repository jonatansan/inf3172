#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv){

	if( !ifstream(argv[1]) ){
		ofstream fileToCreate(argv[1]); 
		cout<<"Fichier créé"<<endl;
	} else {
		cout<<"Impossible de créer le fichier"<<endl; 
	}
	return 0;
}
