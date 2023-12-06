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

    // Variables específicas del cocinero
    
    int capacidad_heladera = 25;


    // LLenar heladera

do{

    if (shared_data->heladera==0);{

        while (shared_data->heladera < 25){
        sem_wait(&shared_data->sem2);

            printf("Repostero: Preparando postres...\n");
            shared_data->heladera += 1;
            shared_data-> postres_preparados += 1;
            printf("Capacidad heladera: %d\n", shared_data->heladera);
            printf("Repostero: Postres preparados: %d\n", shared_data-> postres_preparados);
            sleep(1);
            
        sem_post(&shared_data->sem2);
        }

    }
          
        
 } while (shared_data->valor==1);


     // Desvincular y liberar la memoria compartida
    shmdt(shared_data);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Programa terminado\n");

    return 0;

}