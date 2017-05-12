#include "../include/header.h"

void error(char error[]) {	
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);	
}

void clean(char * buff) {
	int i;
	for(i = 0; i < strlen(buff); i++){
		buff[i] = '\0';
	}
}

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

void listarTickets(char tickets[]) {

	char *token1, *token2;
	char *saveptr1, *saveptr2;

	token1 = strtok_r(tickets, "-", &saveptr1);
   
	while (token1 != NULL)
	{
		token2 = strtok_r(token1, "|", &saveptr2);	
		
		while (token2 != NULL)
		{
			printf("\n%s", token2);
			token2 = strtok_r(NULL, "|", &saveptr2);
		}

		token1 = strtok_r(NULL, "-", &saveptr1);
	}
}
