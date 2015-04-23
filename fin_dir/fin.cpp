#include <iostream>
#include <unistd.h>
#include <sstream>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>


int compte_l_c(FILE *f){

	char lecture;
	int nbLigne = 0; 



	while(!feof(f)){

		fscanf(f, "%c", &lecture);

		if(!feof(f) && lecture == '\n'){

			nbLigne++;
        }

		
	}

	return nbLigne;
}

void affichage(FILE* f, int numLigne, int totLigne){

	char lecture;
	int nbLigne = 0;
	int ligneAffichage = totLigne - numLigne; 
	
	rewind(f);

	while(!feof(f)){
		
		while(nbLigne <= ligneAffichage){
			fscanf(f, "%c", &lecture);

			if(!feof(f) && lecture == '\n'){

				nbLigne++;
		    }
        }
        fscanf(f, "%c", &lecture);
        
        printf("%c", lecture); 	
	}

}


int main(int argc, char** argv){

	FILE* f;
	
	std::string s_arg = argv[1]; 
	
	unsigned int posArg = s_arg.find(" ");//Split le num Ligne et le fichier
	
	std::string s_numLigne = s_arg.substr(0, posArg);
	std::string s_file = s_arg.substr(posArg+1, s_arg.size()); 
	
	if( !isdigit(s_numLigne[0])){
	
		std::cout<<"Argument invalide"<<std::endl;
		return -1; 
	}
	
	int numLigne = stoi(s_numLigne);	
	
	if( (f = fopen(s_file.c_str(), "r")) == NULL){
	
		std::cout<<"Fichier introuvable"<<std::endl; 
		return -1; 
	} 
	
	affichage(f,numLigne, compte_l_c(f)); 
	
	fclose(f);
	
	return 0;
}
