#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUMEROENFERMERAS 5

int calculaAleatorios(int min, int max);
void funcionComprobarAnestesia(int sig);
void funcionGenerarUnoOCero(int sig);
void avisarTodasLasEnfermeras(int sig);
void handlerSIGUSR2();
	

int main(){
	
	int i=0, status;
	pid_t pid[NUMEROENFERMERAS];


	/*En el bucle creamos los hijos y ejecutamos su código*/
	for(i=0; i<NUMEROENFERMERAS; i++){
		pid[i]=fork();
		if(pid[i]==-1){
			perror("Error en la llamada al fork()\n");
			exit(0);
		}else if(pid[i]==0){
			signal(SIGUSR1,funcionComprobarAnestesia);
			signal(SIGUSR2, funcionGenerarUnoOCero);
			printf("Soy la enfermera %d\n", getpid());
			pause();
			exit(0);
		}
	}


	/*Código del padre*/


	printf("Soy la doctora %d\n", getpid());
	for(i=0;i<NUMEROENFERMERAS;i++){
		printf("Hoy ha venido a trabajar la enfermera %d\n",pid[i]);
	} 

	/*La doctura avisa a una enfermera y espera a recibir SIGUSR1 para avisar a todas*/
	signal(SIGUSR1,avisarTodasLasEnfermeras);
	sleep(0.5);
	int enfermeraComprobarAnestesia=calculaAleatorios(0,NUMEROENFERMERAS-1);
	printf("%d\n",enfermeraComprobarAnestesia );
	printf("Voy a enviar una señal a %d\n", pid[enfermeraComprobarAnestesia]);

	kill(pid[enfermeraComprobarAnestesia], SIGUSR1);
	pause();
	/*La doctora notifica a las enfermeras*/
	for(i=0;i<NUMEROENFERMERAS;i++){
		kill(pid[i], SIGUSR2);
	}
	return 0;
}


/* Se genera un número aleatorio entre min y max ambos incluidos*/
int calculaAleatorios(int min, int max){
	srand(time(NULL));
	return rand()%(max-min+1)+min;
}

void funcionComprobarAnestesia(int sig){
	printf("Soy la enfermera %d he recibido SIGUSR1\n", getpid());
	int tiempoQueTardaAnestesia=calculaAleatorios(1,12);
	sleep(tiempoQueTardaAnestesia);
	printf("El paciente está anestesiado, ha tardado %d segundos\n", tiempoQueTardaAnestesia);
}

void avisarTodasLasEnfermeras(int sig){
	printf("Iniciando la operación\n");

}

void funcionGenerarUnoOCero(int sig){
	int numAleatorio=calculaAleatorios(0,1);
	printf("La enfermera %d ha generado un %d\n",getpid(),numAleatorio);
}
