CC = g++ -pthread
CFLAGS = -Wall -std=c++11
DIR = ~/inf3172/bin

all :
		mkdir -p ~/inf3172/bin
		$(CC) $(CFLAGS) -o $(DIR)/tsh main.cpp
		$(CC) $(CFLAGS) -o $(DIR)/list list_dir/list.cpp
		$(CC) $(CFLAGS) -o $(DIR)/new new_dir/new.cpp
		$(CC) $(CFLAGS) -o $(DIR)/newdir newdir_dir/newdir.cpp
		$(CC) $(CFLAGS) -o $(DIR)/rmall rmall_dir/rmall.cpp
		$(CC) $(CFLAGS) -o $(DIR)/size size_dir/size.cpp
		
		$(CC) $(CFLAGS) -o $(DIR)/lc lc_dir/lc.cpp
		$(CC) $(CFLAGS) -o $(DIR)/fin fin_dir/fin.cpp
		$(CC) $(CFLAGS) -o $(DIR)/edit edit_dir/edit.cpp
