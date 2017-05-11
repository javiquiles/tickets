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
#include <time.h>

#define BUF_SIZE 1024

void clean(char *);

char * listTickets();

void editarTicket(char [], char []);

void insertTicket(char [], char []);

void registrar(char [], char []);

char * obtenerFecha();

void error(char []);


