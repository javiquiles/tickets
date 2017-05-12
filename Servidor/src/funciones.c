#include "../include/header.h"

void clean(char * buff){
	int i;
	for(i = 0; i < strlen(buff); i++){
		buff[i] = '\0';
	}
}

char * obtenerFecha() {
	char *fecha = (char *) malloc (128 * sizeof(char));

	time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    strftime(fecha, 128, "%d/%m/%y %H:%M:%S", tlocal);
    
	return fecha;
}

void registrar(char buf[], char ip[]) {

	FILE *log;
	log = fopen("db/log.txt", "a+");

	if (log == NULL) { fputs ("File error",stderr); exit(1); }
	
	fputs(ip, log);
	fputs(" | ", log);
	fputs(obtenerFecha(), log);
	fputs(" | ", log);
	fputs(strtok(buf, "|"), log);
	fputs("\n", log);
	
	fclose (log);
}

char * enviarTickets() {

	char caracteres[100];
	char aux[200];
	char * tickets = (char *) malloc (BUF_SIZE * sizeof(char));

	clean(tickets);
	clean(aux);
	clean(caracteres);

	printf("linea 48\n");

	FILE *db;
	db = fopen("db/tickets.txt", "r");

	if (db == NULL) { fputs ("File error",stderr); exit(1); }

	while (feof(db) == 0) 
	{
		fgets(caracteres, 100, db);
		*(caracteres+(strlen(caracteres)-1))='\0';		
		//caracteres[strlen(caracteres)-1] = '\0';
		printf("%s", caracteres);
		sprintf(tickets, "%s-%s", tickets, caracteres);
		clean(caracteres);
 	}

    fclose(db);

	return tickets;
}

void editarTicket(char ticket[], char ip[]){
	
	char caracteres[100], aux[100];
	char newId[3], oldId[3];
	key_t key;
	struct sembuf operacion;
	int sem_id;

	if((key = ftok("db/tickets.txt", 1)) < 0){
		error("ftok");
		exit(1);
	}

	if ((sem_id = semget(key, 1, IPC_CREAT | 0777)) < 0) {
		error("semget");
		exit(1);
	}

	operacion.sem_num = 0;
	operacion.sem_op = -1;
	operacion.sem_flg = 0;

	if (semop(sem_id, &operacion, 1) == -1) {
		error("semop");
		exit(1);
    }

	strcpy(aux, ticket);
	strtok(aux, "|");


	strcpy(newId, strtok(NULL, "|"));
	while(strtok(NULL, "|") != NULL){
	
	}	
	
	FILE *tmp;
	tmp = fopen("db/tmp.txt", "w+");
	if (tmp==NULL) {fputs ("File error",stderr); exit (1);}	
	
	FILE *db;
	db = fopen("db/tickets.txt", "r+");
	if (db==NULL) {fputs ("File error",stderr); exit (1);}
	

	while(feof(db) == 0){
		fgets(caracteres, 100, db);
		strcpy(aux, caracteres);

		if(strcmp(caracteres, "\0") == 0){
			goto cerrar;
		}
		
		strtok(caracteres, "|");
		strcpy(oldId, strtok(NULL, "|"));

		if((strcmp(oldId, newId)) != 0){
			fputs(aux, tmp);
		}else if((strcmp(oldId, newId)) == 0){
			sprintf(caracteres, "%s%s", ip, ticket+1);
			fputs(caracteres, tmp);
			fputs("\n", tmp);
		}
		
		clean(caracteres);
	}

	cerrar:
		rewind(db);
		rewind(tmp);

		while(feof(tmp) == 0){
			fgets(caracteres, 100, tmp);
			if((strcmp(caracteres, "")) != 0){
				fputs(caracteres, db);
				clean(caracteres);
			}
		}

		fclose (db);
		fclose(tmp);

		if(remove("db/tmp.txt") != 0){
			error("remove");
		}

		printf("1/2. Cerrando bases");

		operacion.sem_op = 1;
		if (semop(sem_id, &operacion, 1) == -1) {
			error("semop");
		}
}


void guardarTicket(char buf[], char ip[]){

	int count = 0;
	char caracteres[100];

	FILE *db;

	db = fopen("db/tickets.txt", "a+");
	if (db==NULL) {error("fopen"); exit (1);}

	while(feof(db) == 0){
		fgets(caracteres, 100, db);
		count++;
	}

	sprintf(caracteres, "%d|", count);
	fputs(ip, db);
	fputs("|", db);
	fputs(caracteres, db);
	fputs(buf+2, db);
	fputs("\n", db);

	fclose (db);
	
}

void error(char error[]){
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);
}
