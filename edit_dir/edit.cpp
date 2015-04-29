#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <vector>
#include <ctype.h>
#include <fstream>

std::vector<std::string> vector_string; 

typedef struct thread_args{
	pthread_t t_input; 
	pthread_t t_write; 
	bool t_input_finish = false; 
	pthread_mutex_t lock_self = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t lock_input = PTHREAD_MUTEX_INITIALIZER; 
	std::string file; 
} thread_args;

void* write_thread(void* args){

	bool firstmaj = true; 
	bool spaceNeeded = false; 
	
	
	access((*(thread_args*)args).file.c_str(), W_OK); 
					
	if(errno == EACCES){
			
		printf("Permission d'écrire insuffisante pour le fichier : %s\n", (*(thread_args*)args).file.c_str()); 
		exit(1); 
					
	}
	
	std::ofstream wfile;
	
	wfile.open( (*(thread_args*)args).file, std::ios::trunc);
	wfile.close();  
	
	
	while( ! (*(thread_args*)args).t_input_finish || vector_string.size() !=0){
		
		pthread_mutex_lock(& (*(thread_args*)args).lock_input); 
		
		if(vector_string.size() != 0){
		
			std::string s = vector_string.front(); 

			
			if(s.size() == 0){
				vector_string.erase(vector_string.begin()); 
				pthread_mutex_unlock(& (*(thread_args*)args).lock_input);
				continue;
			}
			
			if(s.compare("\e") == 0){
				vector_string.erase(vector_string.begin()); 
				continue; 
			}
			
			wfile.open( (*(thread_args*)args).file, std::ios::in | std::ios::app); 
	
			if(spaceNeeded && s[0] != '!' && s[0] != '.' && s[0] != '?'){
				wfile<<' '; 
			}
			
			spaceNeeded = true; 
			
			if(firstmaj){
		
				for(unsigned int i = 0; i < s.size()-1; ++i){
			
					if(s[i] >= 'a' && s[i] <= 'z'){
						s[i] = toupper(s[i]);
						firstmaj = false; 
						break; 
					}
					
					if(s[i] >= 'A' && s[i] <= 'Z'){
						firstmaj = false; 
						break; 
					}
				}
			}
		
		
			//Traitement de la string
			for(unsigned int i = 0; i < s.size()-1; ++i){
		
				if(s[i] == '!' || s[i] == '.' || s[i] == '?'){
			
					if(s[i+1] != ' '){
			
						s.insert(i+1, " ");
		
					}
				
					if(s[i+2] >= 'a' && s[i+2] <= 'z'){
				
						s[i+2] = toupper(s[i+2]); 
					}
				}
			}
		
			
			for(unsigned int i = s.size() - 1; i >=0; --i){
			
				if(s[i] == '!' || s[i] == '.' || s[i] == '?'){
	
					firstmaj = true; 
					break;
					
				} 
				else if(s[i] == ' ') continue;
				else break; 
			}
			wfile<<s; 
			vector_string.erase(vector_string.begin()); 
		}
		
		wfile.close(); 
		pthread_mutex_unlock(& (*(thread_args*)args).lock_input);
	}
	
	//printf("Exit w!\n"); 
	pthread_exit(NULL); 
}


void* input_thread(void* args){

	std::string input; 

	do{
		
		if(!getline(std::cin, input)){
			(*(thread_args*)args).t_input_finish = true; 
			pthread_exit(NULL); 
		}
		
		
		pthread_mutex_lock(& (*(thread_args*)args).lock_input);
		
		vector_string.push_back(input); 
		
		pthread_mutex_unlock(& (*(thread_args*)args).lock_input);
		
	}while(input.compare("\e") != 0);
	
	(*(thread_args*)args).t_input_finish = true; 
	//printf("Exit r!\n"); 
	pthread_exit(NULL);
}


int main(int argc, char** argv){

	
	pthread_attr_t attr;
 	pthread_attr_init( &attr );
 	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );
	
	thread_args args; 
	
	args.file = std::string(argv[1]); 
	
	pthread_mutex_init(& (args.lock_input), NULL ); 
	

	pthread_t t_input; 
	pthread_t t_write; 
	
	//Create thread for reading
	pthread_create(&t_input, &attr, &input_thread, (void*) &args); 

	
	//Create thread for writing
	pthread_create(&t_write, &attr, &write_thread, (void*) &args); 
	
	
	pthread_join(t_input, NULL); 
	pthread_join(t_write, NULL); 
	
	pthread_mutex_destroy( &(args.lock_self)); 	

	return 0;
}
