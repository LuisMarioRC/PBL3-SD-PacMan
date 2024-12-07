all: 
	gcc -w -Wall mouse.c main.c -g -o main mapmem.s -lintelfpgaup -lpthread -lrt -std=c99
	sudo ./main
