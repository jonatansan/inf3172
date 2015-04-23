#include <iostream>
#include <unistd.h>
#include <sstream>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

using namespace std; 

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
			
			//pid_t parent = getpid();
			
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
			
				char c_command[1024];
				strcpy(c_command, fullPath.c_str());
			
				char c_arg[1024];
				strcpy(c_arg, arg.c_str());
			
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
