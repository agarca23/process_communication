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
	printf("Hola");
	int x,i=0, status;
	pid_t pid[NUMEROENFERMERAS];


	/*En el bucle creamos los hijos y ejecutamos su código*/
	for(i=0; i<NUMEROENFERMERAS; i++){
		pid[i]=fork();
		if(pid[i]==-1){
			perror("Error en la llamada al fork()");
			exit(0);
		}else if(pid[i]==0){
			signal(SIGUSR1,funcionComprobarAnestesia);
		//	signal(SIGUSR2,handlerSIGUSR2);
			printf("Soy la enfermera %d", getpid());
			pause();
			kill(getppid(),SIGUSR1);
			exit(0);
		}
	}


	/*Código del padre*/

	signal(SIGUSR1,avisarTodasLasEnfermeras);
	int numeroAleatorio=calculaAleatorios(0,NUMEROENFERMERAS);
	kill(numeroAleatorio,SIGUSR1);
	pause();
	printf("Saliendo\n");
	
	

	return 0;
}


/* Se genera un número aleatorio entre min y max ambos incluidos*/
int calculaAleatorios(int min, int max){
	srand(time(NULL));
	return rand()%(max-min+1)+min;
}


/*función en la que el hijo espera a que el paciente se duerma
 *y envía la señal al padre*/
void funcionComprobarAnestesia(){
	printf("Enfermera %d esperando a que el paciente se durerma.\n", getpid());
	sleep(calculaAleatorios(1,12));
	printf("Paciente anestesiado\n" );
	printf("Enfermera %d envia la señal SIGUSR1 a la doctora.\n", getpid());
	kill(getppid(), SIGUSR1);
}

/*función que genera un uno o un cero de manera aleatoria*/
int funcionGenerarUnoCeros(){
	
}

void handlerSIGUSR2(){

}


void avisarTodasLasEnfermeras(){

}
