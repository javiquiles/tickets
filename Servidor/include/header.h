#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/sem.h>

#define BUF_SIZE 1024

void clean(char * );

char * listTickets(char []);

void editarTicket(char [], char []);

void insertTicket(char [], char []);

void error(char []);


