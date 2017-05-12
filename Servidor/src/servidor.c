#include "../include/header.h"

 
int main(int argc, char** argv) {
 
	char ipstr[INET6_ADDRSTRLEN];

    int sockid, conn_sock, count;
    struct sockaddr_in direccion;
    char buffer[BUF_SIZE];
    int pid;
	struct sockaddr_storage client;
	socklen_t addr_size = sizeof(client);

	if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\nUSAGE: <executable> <port>");
         exit(1);
     }

    //creamos el socket inet-stream
    if((sockid=socket(PF_INET,SOCK_STREAM,0)) < 0) {
        error("socket");
    }

    //seteamos la direccion en la que va a escuchar
    direccion.sin_family=AF_INET; //address family
    direccion.sin_port=htons(atoi(*(argv+1)));      //atoi ascii to integer
    direccion.sin_addr.s_addr=htonl(INADDR_ANY); //0.0.0.0

    //asociamos el socket con la direccion (bind)
    if((bind(sockid, (struct sockaddr *)&direccion, sizeof(direccion))) < 0){
        error("bind");
    }

    // seteamos la cantidad de conexiones concurrentes en cola
    listen(sockid,1);

    //dejamos escuchando al proceso en el socket ip:puerto
    while(conn_sock=accept(sockid,(struct sockaddr *)&client, &addr_size)) {
        if (conn_sock<0) {
                error("accept");
        }

        if (!(pid=fork())) { 
            while(count=recv(conn_sock,buffer,BUF_SIZE,0)) {
                if (count < 0) {
                    error("recv");
                }			
			
				if (client.ss_family == AF_INET) {
					struct sockaddr_in *s = (struct sockaddr_in *)&client;
					inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
				}

				switch(buffer[0]) {
					case 'i':
						//insertTicket(buffer, ipstr);
						registrar(buffer, ipstr);
					break;

					case 'l':
						strcpy(buffer, listTickets());
						send(conn_sock, buffer, strlen(buffer), 0);
					break;

					case 'e':
						//editarTicket(buffer, ipstr);
					break;

					default:
						exit(1);
					break;
				}
				
				
            }
            close(sockid);
            exit(0);
        }

        printf("Conexion delegada al proceso hijo No. %d\n",pid);
        waitpid(pid, NULL, WNOWAIT);
    }
    return 0;
}
