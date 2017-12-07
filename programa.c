#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUMEROENFERMERAS 5

int calculaAleatorios(int min, int max);
void funcionComprobarAnestesia();
int funcionGenerarUnoCeros();
void avisarTodasLasEnfermeras();
void handlerSIGUSR2();
	

int main(){

	int i=0;
	pid_t pid[NUMEROENFERMERAS];


	/*En el bucle creamos los hijos y ejecutamos su código*/
	for(i=0; i<NUMEROENFERMERAS; i++){
		pid[i]=fork();
		if(pid[i]==-1){
			perror("Error en la llamada al fork()\n");
			exit(0);
		}else if(pid[i]==0){
			printf("Soy la enfermera %d\n", getpid());
			exit(0);
		}
	}


	/*Código del padre*/


	printf("Soy la doctora %d\n", getpid());
	
	

	return 0;
}


/* Se genera un número aleatorio entre min y max ambos incluidos*/
int calculaAleatorios(int min, int max){
	srand(time(NULL));
	return rand()%(max-min+1)+min;
}
