#include <iostream>
#include <unistd.h>
#include <sstream>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>

#include <pthread.h>

typedef struct thread_arg{

char d_name[256]; 
int nb; 
bool finish = false; 
} thread_arg; 

pthread_t t_id[10];

void* count_line(void* args){

	char lecture;
	int nbLigne = 0; 
	
	int r_perm = access( (*(thread_arg*)args).d_name, R_OK); 
					
	if(r_perm != 0){
					
		printf("Permission insuffisante de lecture pour : %s", (*(thread_arg*)args).d_name); 
		(*(thread_arg*)args).finish = true; 
		pthread_exit(args); 
					
	}
	
	FILE* f;
	if( (f = fopen( (*(thread_arg*)args).d_name, "r")  )  == NULL){
	
		printf("fopen error : %d", errno);
		(*(thread_arg*)args).finish = true; 
		pthread_exit(args); 
	
	} 
	
	char currentdir[1024];
	if(getcwd(currentdir, sizeof(currentdir))){
		//On ne fait rien, on affiche le currentdir plus tard
	} else {
		printf("getcwd() error : %d", errno);
		(*(thread_arg*)args).finish = true; 
		pthread_exit(args); 
	}


	while(!feof(f)){

		fscanf(f, "%c", &lecture);

		if(!feof(f) && lecture == '\n'){

			nbLigne++;
		}
	}
	
	printf("%s/%s contient %d lignes\n", (char*) currentdir, (*(thread_arg*)args).d_name, nbLigne); 
	
	fclose(f); 
	
	(*(thread_arg*)args).finish = true; 
	pthread_exit(args); 
}


int main(int argc, char** argv){

	int count = 0; 
	
	thread_arg* thread_args[10]; 
	
	for(int i = 0; i < 10; i++){
	
		thread_args[i] = new thread_arg; 
	}
	
	DIR* dirToRead;
	struct dirent*  dirEntToOpen;
	
	dirToRead = opendir(".");
	
	while( (dirEntToOpen = readdir(dirToRead)) ){



			if( dirEntToOpen->d_type == DT_REG){
			
				if(count >= 10){
				
					bool newThread = false; 
				
					while(!newThread){
					
						for(int i = 0; i < 10; ++i){
				
							if(thread_args[i]->finish){
					
								delete thread_args[i];
								thread_args[i] = new thread_arg; 
						
								strcpy(thread_args[i]->d_name, dirEntToOpen->d_name); 
								thread_args[i]->nb = i; 
								thread_args[i]->finish = false; 

								pthread_create(&(t_id[i]), NULL, &count_line, (void*) thread_args[i]); 
								newThread = true; 
							}
							
							if(newThread) break; 
				
						}
					}
					 
				} else {

					strcpy(thread_args[count]->d_name, dirEntToOpen->d_name); 
					thread_args[count]->nb = count; 

					pthread_create(&(t_id[count]), NULL, &count_line, (void*) thread_args[count]); 
					count++;
				} 
			}
			
	}
	
	for(int i = 0; i < count; ++i){
	
		pthread_join(t_id[i], NULL); 
	}
	
	for(int i = 0; i < 10; i++){
	
		delete(thread_args[i]);
	}
	
	
	return 0;
}
