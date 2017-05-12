#include "../include/header.h"

void clean(char * buff) {
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

	if (log == NULL) { fputs ("File error", stderr); exit(1); }
	
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
	char * tickets = (char *) malloc (BUF_SIZE * sizeof(char));

	clean(tickets);
	clean(caracteres);

	FILE *db;
	db = fopen("db/tickets.txt", "r");

	if (db == NULL) { error("db"); }

	while (feof(db) == 0) 
	{
 		fgets(caracteres, 100, db);
		sprintf(tickets, "%s-%s", tickets, caracteres);
		clean(caracteres);
 	}

    fclose(db);

	return tickets;
}

/*void editarTicket(char ticket[], char ip[]) {
	key_t key;
	struct sembuf operacion;
	int id, sem_id;
	
	char dir_file[30]="db/";
	
	//clean(ticket);

	strtok(ticket, "|");
	id = strtoll(strtok(NULL, "|"), NULL, 10);

	printf("Ident: %d", id);

	sprintf(dir_file, "db/%d.txt", id);

	if((key = ftok(dir_file, id)) < 0){
		error("ftok");
		exit(1);
	}

	printf("key: %d", key);

	if ((sem_id = semget(key, 1, IPC_CREAT | 0777)) < 0) {
		error("semget");
		exit(1);
	}

	printf("sem_id: %d", sem_id);

	operacion.sem_num = 0;
	operacion.sem_op = -1; //Bajamos el semaforo (lo ponemos en rojo).
	operacion.sem_flg = 0;

	if (semop(sem_id, &operacion, 1) == -1) {
		error("semop");
		exit(1);
    }

	printf("linea 98");

	sleep(10);
	
	FILE *db;
	db = fopen(dir_file, "w");
	if (db==NULL) {fputs ("File error",stderr); exit (1);}

	fputs(ip, db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs("Pendiente", db);
	fputs("\n", db);
	
	fclose (db);

	operacion.sem_op = 1;
	
	if (semop(sem_id, &operacion, 1) == -1) {
		error("semop");
    }
	
}


void insertTicket(char buf[], char ip[]) {

	key_t key;
	struct sembuf operacion;
	int sem_id;

	//Cuento la cantidad de tickets en mi bd
	struct dirent *dirent;
	DIR *dir;
	long long count = -1;
	char dir_file[30]="db/";

	dir = opendir(dir_file);
	if(dir == NULL){
		error("opendir");
		exit(0);
	}
	
	while((dirent = readdir(dir)) != NULL){
		count++;
	}

	closedir(dir);

	printf("%lld", count);
	
	//Construyo la url hacia la base de datos
	sprintf(dir_file, "%s%lld.txt", dir_file, count);

	printf("dir: %s", dir_file);

	

	
	//Creo el archivo y guardo los datos
	FILE *db;
	db = fopen(dir_file, "w");
	if (db==NULL) {fputs ("File error",stderr); exit (1);}
	
	strtok(buf, "|");
	fputs(ip, db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs(strtok(NULL, "|"), db);
	fputs("\n", db);
	fputs("Pendiente", db);
	fputs("\n", db);
	
	fclose (db);

	if((key = ftok(dir_file, count)) < 0){
		error("ftok");
		exit(1);
	}
	

	if ((sem_id = semget(key, 1, IPC_CREAT | 0777)) < 0) {
		error("semget");
		exit(1);
	}

	if(semctl(sem_id, 0, SETVAL, 1) < 0){
		error("semctl");
		exit(1);
	}

	printf("key: %d | sem_id: %d", key, sem_id);
	
}*/


void error(char error[]){
	fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
	exit(1);
}
