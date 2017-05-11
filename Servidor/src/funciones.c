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
	log = fopen("../db/log.txt", "a+");

	if (log == NULL) { fputs ("File error",stderr); exit(1); }
	
	fputs(ip, log);
	fputs(" | ", log);
	fputs(obtenerFecha(), log);
	fputs(" | ", log);
	fputs(strtok(buf, "|"), log);
	fputs("\n", log);
	
	fclose (log);
}

char * listTickets() {

	char caracteres[100];
	char aux[200];
	char * tickets = (char *) malloc (BUF_SIZE * sizeof(char));

	clean(tickets);
	clean(aux);
	clean(caracteres);

	FILE *db;
	db = fopen("db/tickets.txt", "r");

	if (db == NULL) { fputs ("File error",stderr); exit(1); }

	fgets(caracteres, 100, db);
	caracteres[strlen(caracteres)-1] = '\0';

	while (feof(db) == 0) 
	{
		caracteres[strlen(caracteres)-1] = '\0';
		sprintf(aux, "%s|%s", aux, caracteres);
 		fgets(caracteres, 100, db);
		sprintf(tickets, "%s-%s", tickets, aux);
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

	printf("sem_id: %d\nkey: %d", sem_id, key);

	operacion.sem_num = 0;
	operacion.sem_op = -1; //Bajamos el semaforo (lo ponemos en rojo).
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
	tmp = fopen("db/tmp.txt", "w");
	if (tmp==NULL) {fputs ("File error",stderr); exit (1);}	
	
	FILE *db;
	db = fopen("db/tickets.txt", "r");
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
			printf("puts: %d\n", fputs(aux, tmp));
		}else if((strcmp(oldId, newId)) == 0){
			sprintf(caracteres, "%s%s", ip, ticket+1);
			fputs(caracteres, tmp);
			fputs("\n", tmp);
		}
		
		clean(caracteres);
	}

	cerrar:

		fclose (db);
		fclose(tmp);

		if(remove("db/tickets.txt") != 0){
			error("remove");
			exit(1);
		}
		if(rename("db/tmp.txt", "db/tickets.txt") != 0){
			error("rename");
		}

				printf("1/2. Cerrando bases");

		operacion.sem_op = 1;
	
		if (semop(sem_id, &operacion, 1) == -1) {
			error("semop");
		}

	
}


void insertTicket(char buf[], char ip[]){

	int count = 0;
	char caracteres[100];

	//Creo el archivo y guardo los datos
	FILE *db;

	db = fopen("db/tickets.txt", "r+");
	if (db==NULL) {fputs ("File error", stderr); exit (1);}

	while(feof(db) == 0){
		fgets(caracteres, 100, db);
		count++;
	}

	printf("%d",count);

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
