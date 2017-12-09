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
void inicioDeOperacion(int sig);
	

int main(int argc, char *argv[]){
	/*Si el numero de argumentos no es correcto informamos y finalizamos la ejecucion*/
	if (argc!=3){
		printf("Debe pasar tres argumentos\n");
		exit(0);
	}

	/*pasamos las cadenas de caracteres de argv[] a int*/
	int minimo=atoi(argv[1]);
	int maximo=atoi(argv[2]);

	
	int i=0, status;
	pid_t pid[NUMEROENFERMERAS];


	/*En el bucle creamos los hijos y ejecutamos su código*/
	for(i=0; i<NUMEROENFERMERAS; i++){
		pid[i]=fork();
		if(pid[i]==-1){
			perror("Error en la llamada al fork().\n");
			exit(0);
		}else if(pid[i]==0){
			signal(SIGUSR1,funcionComprobarAnestesia);
			signal(SIGUSR2, funcionGenerarUnoOCero);
			printf("Soy la enfermera %d.\n", getpid());
			printf("Soy %d: Esperando a recibir una señal.\n", getpid() );
			pause();
			pause();
			exit(0);
		}
	}


	/*CÓDIDO DEL PADRE*/

	printf("Soy la doctora %d.\n", getpid());
	for( i=0 ; i<NUMEROENFERMERAS ; i++ ){
		printf("Hoy ha venido a trabajar la enfermera %d.\n", pid[i] );
	} 

	/*La doctora avisa a una enfermera y espera a recibir SIGUSR1 para avisar a todas*/
	signal( SIGUSR1, inicioDeOperacion );
	sleep(2);
	int enfermeraComprobarAnestesia=calculaAleatorios( 0, NUMEROENFERMERAS-1 );
	printf("\n--------------------------------------------------\n");
	printf("\nSoy la doctora %d: voy a enviar una señal a %d.\n", getpid(), pid[enfermeraComprobarAnestesia] );
	kill( pid[enfermeraComprobarAnestesia], SIGUSR1 );
	printf("Soy la doctora %d: Esperando a recibir una señal.\n", getpid() );
	pause();
	printf("Soy la doctora %d: Notifico a todas las enfermeras por medio de SIGUSR2.\n", getpid());

	/*La doctora notifica a las enfermeras*/
	for( i=0 ; i<NUMEROENFERMERAS ; i++){

		kill( pid[i], SIGUSR2 );
		sleep(1);
	}

	/*Esperamos por cada uno de los hijos y recogemos el estado*/
	int numeroCeros=0;
	int numeroUnos=0;
	for( i=0 ; i<NUMEROENFERMERAS ; i++ ){
		wait(&status);
		if(WIFEXITED(status)){
			if(WEXITSTATUS(status)==1){
				numeroUnos++;
			}
			if(WEXITSTATUS(status)==0){
				numeroCeros++;
			}
		}
	}

    int numeroParaExitoOperacion=calculaAleatorios(minimo, maximo);
    printf("El numero de 1s es %d.\n", numeroUnos );
    printf("El numero generado por la doctora es %d.\n",numeroParaExitoOperacion );

    /*Calculamos si la operación ha tenido exito*/
	if(numeroUnos>=3&&numeroParaExitoOperacion%2==0){
		printf("La operación ha resultado satisfactoria.\n\n");
	}else{
		printf("El paciente ha muerto.\n\n" );
	}
	return 0;
}


/* Se genera un número aleatorio entre min y max ambos incluidos*/
int calculaAleatorios(int min, int max ){
	srand(time(NULL));
	return rand()%(max-min+1)+min;
}

/*Esperamos a que el paciente esté anestesiado y notificamos al padre*/
void funcionComprobarAnestesia(int sig ){
	printf("Soy la enfermera %d he recibido SIGUSR1.\n", getpid());
	int tiempoQueTardaAnestesia=calculaAleatorios( 1, 12 );
	sleep(tiempoQueTardaAnestesia);
	printf("Soy %d: El paciente está anestesiado, ha tardado %d segundos.\n", getpid(), tiempoQueTardaAnestesia );
	printf("Soy %d: Notifico a la doctora por medio de SIGUSR1.\n", getpid() );
	kill( getppid(), SIGUSR1 );
}

/*Se ejecuta cuando el padre recibe SIGUSR1*/
void inicioDeOperacion(int sig ){
	printf("Soy la doctora %d: La enfermera me ha notificado que el paciente ya está anestesiado.\n", getpid());
	printf("\n--------------------------------------------------\n");
	printf("\nIniciando la operación.\n");

}


/*Se ejecuta cuando recibimos SIGURS2, calcula un numero entre cero y uno. Finalizan las enfermeras*/
void funcionGenerarUnoOCero(int sig){
	int numAleatorio=calculaAleatorios(0,1);
	printf("Soy la enfermera %d y he generado un %d.\n", getpid(), numAleatorio );
	exit(numAleatorio);
}
