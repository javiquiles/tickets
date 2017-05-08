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

char * ingresarTicket(int op, char id_ticket[]) {

	char *ticket = (char *) malloc (BUF_SIZE * sizeof(char));
	char fecha[20];
	char titulo[20];
	char autor[30];
	char descripcion[50];
	
	printf ("\nFecha de creacion: ");
	scanf("%s", fecha);
	printf ("Titulo: ");
	scanf("%s", titulo);
	printf ("Autor: ");
	scanf("%s", autor);
	printf ("Descripcion: ");
	scanf("%s", descripcion);
	
	if (op == 'i')
		sprintf (ticket, "%c|%s|%s|%s|%s", op, fecha, titulo, autor, descripcion);
	else
		sprintf (ticket, "%c|%s|%s|%s|%s|%s", op, id_ticket, fecha, titulo, autor, descripcion);

	return ticket;
}

void listarTickets(char tickets[BUF_SIZE]) {

	char *token1, *token2;
	char *saveptr1, *saveptr2;

	token1 = strtok_r(tickets, "-", &saveptr1);
   
	while (token1 != NULL)  {

		token2 = strtok_r(token1, "|", &saveptr2);
		printf("\n");		
		
		while (token2 != NULL) {

			printf("%s\n", token2);
			token2 = strtok_r(NULL, "|", &saveptr2);
		}

		token1 = strtok_r(NULL, "-", &saveptr1);
	}
}
 
int main(int argc, char** argv) {
 
    char *error;
	/*char *token1, *token2;
	char *saveptr1, *saveptr2;*/

    int sockid, conn_sock, count;
    struct sockaddr_in direccion;
    char buffer[BUF_SIZE];

	int c;

    if((sockid=socket(PF_INET,SOCK_STREAM,0)) < 0) {
        error="socket";
        goto err;
    }

    direccion.sin_family=AF_INET; //address family
    direccion.sin_port=htons(atoi(*(argv+2)));
    direccion.sin_addr.s_addr=inet_addr(*(argv+1));

    if (connect(sockid, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        error="connect";
        goto err;
    }

	count = strlen(buffer);

	c = getopt (argc, argv, "ile:"); 
    printf("Bienvenido!\n");

	switch (c)
	{
		case 'i':
			strcpy(buffer, ingresarTicket(c, NULL));

			if ((send(sockid, buffer, count, 0)) < 0) {
				error = "send";
				goto err;
			}
			
			printf("\nTicket ingresado.\n");
		break;
		
		case 'l':
			sprintf (buffer, "%d", c);

			if ((send(sockid, buffer, count, 0)) < 0) {
				error = "send";
				goto err;
			}

			recv(sockid, buffer, BUF_SIZE, 0);
			if (count < 0) {
                error="recv";
                goto err;
            }

			listarTickets(buffer);
		break;

		case 'e':
			strcpy(buffer, ingresarTicket(c, argv[4]));

			if ((send(sockid, buffer, count, 0)) < 0) {
				error = "send";
				goto err;
			}

			printf("\nTicket editado.\n");
		break;

		default:
			printf ("\nOpcion no valida.\n");
			printf ("\nIngrese: %s + <-i:insertar, -l:listar, -e:editar>\n", argv[0]);
			error = "send";
			goto err;
		break;
	}
	
	
    /*if ((send(sockid, buffer, count, 0)) < 0) {
        error = "send";
        goto err;
    }

	count=recv(sockid, buffer, BUF_SIZE, 0);

	token1 = strtok_r(buffer, "-", &saveptr1);
   
	while (token1 != NULL) 
	{
		token2 = strtok_r(token1, "|", &saveptr2);
		printf("\n");		
		
		while (token2 != NULL) 
		{
			printf("%s\n", token2);
			token2 = strtok_r(NULL, "|", &saveptr2);
		}

		token1 = strtok_r(NULL, "-", &saveptr1);
	}*/

    close(sockid);

    return 0;

err:
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);
}
