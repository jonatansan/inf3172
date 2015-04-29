#include <iostream>
#include <unistd.h>
#include <sstream>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 

using namespace std; 

//Function to call at the end of redirection
void close_input(void){

	fclose(stdin); 

}

//Function to call at the end of redirection
void close_output(void){

	fclose(stdout); 

}

string getCommand(){

	string input; 

	cout<<"tsh> "; //Affichage du prompt
	
	getline(cin,input); //Capture de la ligne de commande
	
	stringstream ss(input);
	return ss.str(); 

}

void executeLineCommand(string& lineCommand){
		
		string command;
		string arg;
		
		//Parsing de la ligne de commande en commande et ses arguments
		command = lineCommand.substr(0, lineCommand.find(" ")); // Obtenir le premier mot de la commande
		
		unsigned int posArg = lineCommand.find(" ");//Trouver où se situe l'argument
		
		if(posArg != string::npos) {// Vérifier qu'un argument existe
			arg = lineCommand.substr(posArg+1); 
		}
			
		//Interprétation de la commande
			
		//-------- exit --------
		if(command.compare("exit") == 0){
			//On ne fait rien, l'utilisateur veut simplement quitter
			return;
		}
			
		//-------- cd --------
		else if(command.compare("cd") == 0) {
		 
			
			if(chdir(arg.c_str()) == -1){
				
				cout<<"Répertoire introuvable"<<endl;
			}	
		}
		
		//-------- cdir <répertoire de destination> --------
		 else if(command.compare("cdir") ==0){
			char currentdir[1024];
			if(getcwd(currentdir, sizeof(currentdir))){
				fprintf(stdout, "Répertoire courant : %s \n", currentdir); 
			} else {
				printf("getcwd() error : %s", strerror(errno));
			}
	
		} 		
		else {
			
			pid_t pid = fork();
			
			if(pid == -1){
			
				cout<<"Errno : "<<strerror(errno)<<endl;
			}
			
			else if(pid != 0){ // parent
			
				int status;
				waitpid(pid, &status, 0);
			}
			
			else { //Enfant

				char login_c[1024]; 
				
				getlogin_r(login_c, 1024); 
				
				string login_str(login_c); 
				
				string fullPath = "/home/" + login_str +"/inf3172/bin/" + command;  
				
				//Change I/O
				
				size_t pos_output = arg.find(">"); 
				size_t pos_input = arg.find("<"); 

				if(pos_output != std::string::npos){
					//Il y a redirection dans un fichier ! 
					
					std::string file_output = arg.substr(pos_output + 2, arg.size()); 
					arg = arg.substr(0, pos_output - 1); 
					
					//Regarder permission
					access(file_output.c_str(), W_OK); 
					
					if(errno == EACCES){
					
						std::cout<<"Permission insuffisante d'écriture pour "<<file_output<<std::endl;
						exit(1); 
					
					}
					
					if( freopen(file_output.c_str(), "w", stdout) == NULL){
					
						std::cout<<"Erreur lors du changement de la sortie standard"<<std::endl; 
						exit(1); 
					} 
					
					atexit(close_output); 
					
				
				} else if (pos_input != std::string::npos){
					//On prend une fichier comme entrée ! 
					
					std::string file_input = arg.substr(pos_input + 2, arg.size()); 
				
					arg = arg.substr(0, pos_input - 1); 
					
					int r_perm = access(file_input.c_str(), R_OK); 
					
					if(r_perm != 0){
					
						std::cout<<"Permission insuffisante de lecture pour "<<file_input<<std::endl;
						exit(1); 
					
					}
					
					if( freopen(file_input.c_str(), "r", stdin) == NULL){
					
						std::cout<<"Erreur lors du changement de l'entrée standard"<<std::endl; 
						exit(1); 
					} 
					
					atexit(close_input); 
					
				
				}
				
				//Fin change I/O
			
				char c_command[1024];
				strcpy(c_command, fullPath.c_str());
			
				char c_arg[1024];
				strcpy(c_arg, arg.c_str());
				
				//Change I/O
			
				char *argv[] = {c_command, c_arg, NULL}; 
				
				int ret = execve(c_command, argv, NULL); 
				
				if(ret == -1){
					cout<<"Programme introuvable"<<endl;
					exit(0); 
				}
				
			}
		}

}

int main(int argc, char** argv){

	string lineCommand; 
	
	do{
		
		lineCommand.clear(); 
		lineCommand = getCommand(); 
		
		if(!lineCommand.empty()){
			executeLineCommand(lineCommand); 
		}
		
	}while(lineCommand.empty() || lineCommand.compare("exit") != 0); 
	
	return 0;
}
