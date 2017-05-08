/*
        Uso:
                ./cliente ip_servidor puerto
*/
 
 
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
 
#define BUF_SIZE 1024
 
int main(int argc, char** argv) {
 
    char *error;

    int sockid, conn_sock, count;
    struct sockaddr_in direccion;
    char buffer[BUF_SIZE];


    if((sockid=socket(PF_INET,SOCK_STREAM,0))<0) {
        error="socket";
        goto err;
    }

    direccion.sin_family=AF_INET; //address family
    direccion.sin_port=htons(atoi(*(argv+2)));
    direccion.sin_addr.s_addr=inet_addr(*(argv+1));

    if(connect(sockid, (struct sockaddr *)&direccion, sizeof(direccion))<0) {
        error="connect";
        goto err;
    }

    printf("Conexion establecida!! Procedemos a enviar datos...\n");

    while(count=read(STDIN_FILENO,buffer,BUF_SIZE)) {

        if((send(sockid, buffer, count, 0))<0) {
                error="send";
                goto err;
        }
        printf("Dato enviado...\n");
    }

    close(sockid);

    return 0;

err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}
