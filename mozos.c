#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHM_KEY 123456
#define mesadaMAX 27

typedef struct {
   int mesada;
    int heladera;
    int platos_preparados;
    int postres_preparados;
    sem_t sem1;
    sem_t sem2;
    int valor;
} SharedData;

int main() {



    // Esperar hasta obtener la memoria compartida
  int shmid = shmget(SHM_KEY, sizeof(SharedData), 0777);
    if (shmid == -1) {
        perror("Error al obtener la memoria compartida");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);


   

    

    // Variables específicas del mozo
    int capacidad_bandeja = 4;
    int capacidad_bandeja_postres= 6;
    int platos_retirados = 0;
    int postres_retirados = 0;
    int numeroAleatorio = rand() % 2 + 1;
    

    // Ciclo de retiro de platos

while (shared_data->valor==1){

    if (numeroAleatorio==1){

            while (shared_data->mesada > 0 && numeroAleatorio==1) {

                 sem_wait(&shared_data->sem1);
                   
                    // Código del mozo para retirar platos
                    printf("Mozo: Retirando platos...\n");
                    int platos_retirados_en_iteracion = shared_data->mesada < capacidad_bandeja ? shared_data->mesada : capacidad_bandeja;
                    shared_data->mesada -= platos_retirados_en_iteracion;
                    platos_retirados += platos_retirados_en_iteracion;
                
                    printf("Mozo: Platos retirados: %d\n", platos_retirados );
                     numeroAleatorio = rand() % 2 + 1;
                    sleep(1);

                 sem_post(&shared_data->sem1);
    
   
            }
    }

else {

    while (shared_data->heladera>0 && numeroAleatorio==2){
        sem_wait(&shared_data->sem2);
       
        // Código del mozo para retirar postres
        printf("Mozo: Retirando postres...\n");
        int postres_retirados_en_iteracion = shared_data->heladera < capacidad_bandeja_postres ? shared_data->heladera : capacidad_bandeja_postres;
        shared_data->heladera -= postres_retirados_en_iteracion;
        postres_retirados += postres_retirados_en_iteracion;
     
        printf("Mozo: Postres retirados: %d\n", postres_retirados );
         numeroAleatorio = rand() % 2 + 1;
        sleep(1);

        sem_post(&shared_data->sem2);


    }

}

}
    // Desvincular y liberar la memoria compartida
    shmdt(shared_data);

    printf("Mozo: Todos los platos han sido retirados. Cierre del programa.\n");

    return 0;



}
