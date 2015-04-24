#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <vector>

std::vector<std::string> vector_string; 

typedef struct thread_args{
	pthread_t t_input; 
	pthread_t t_write; 
	bool t_input_finish = false; 
	pthread_mutex_t lock_self = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t lock_input = PTHREAD_MUTEX_INITIALIZER; 
} thread_args;

void* write_thread(void* args){

		while( ! (*(thread_args*)args).t_input_finish){
			
			pthread_mutex_lock(& (*(thread_args*)args).lock_input); 
			
			if(vector_string.size() != 0){
			
				std::cout<<vector_string.front()<<std::endl;
			
				vector_string.erase(vector_string.begin()); 
			}
		
			pthread_mutex_unlock(& (*(thread_args*)args).lock_input);
		}
	pthread_exit(NULL); 
}


void* input_thread(void* args){

	std::string input; 

	do{
		
		getline(std::cin, input); 
		
		if( (pthread_mutex_lock(& (*(thread_args*)args).lock_input)) != 0){
		
			//printf("Error lock\n"); 
		}
		
		vector_string.push_back(input); 
		
		if( (pthread_mutex_unlock(& (*(thread_args*)args).lock_input)) != 0){
			//printf("Error unlock\n"); 
		}
		
	}while(input.compare("\e") != 0);
	
	(*(thread_args*)args).t_input_finish = true; 
	pthread_exit(NULL);
}


int main(int argc, char** argv){

	FILE* f;
	
	if( (f = fopen(argv[1], "w")) == NULL){
	
		printf("fopen error : %d", errno);
	}
	
	pthread_attr_t attr;
 	pthread_attr_init( &attr );
 	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE );
	
	thread_args args; 
	
	//pthread_mutex_init(& (args.lock_self), NULL ); 
	pthread_mutex_init(& (args.lock_input), NULL ); 
	

	pthread_t t_input; 
	pthread_t t_write; 
	
	//Create thread for reading
	pthread_create(&t_input, &attr, &input_thread, (void*) &args); 
	
	//pthread_mutex_lock(& (args.lock_self));
	
	//args.t_input = t_input; 
	
	//pthread_mutex_unlock(& (args.lock_self));
	
	//Create thread for writing
	
	pthread_create(&t_write, &attr, &write_thread, (void*) &args); 
	
	//pthread_mutex_lock(& (args.lock_self));
	
	//args.t_write = t_write; 
	
	//pthread_mutex_unlock(& (args.lock_self));
	
	pthread_join(t_input, NULL); 
	pthread_join(t_write, NULL); 
	
	//pthread_mutex_destroy( &(args.lock_input)); 
	//pthread_mutex_destroy( &(args.lock_self)); 	

	return 0;
}
