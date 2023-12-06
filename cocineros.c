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

    // Crear e inicializar la memoria compartida
    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0777);
    if (shmid == -1) {
        perror("Error al crear la memoria compartida");
        exit(EXIT_FAILURE);
    }

     SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    
    shared_data->mesada = 0;
    shared_data->heladera = 25;
    shared_data-> platos_preparados = 0;
    shared_data->postres_preparados=0;
    shared_data->valor=1;

    //iniciar semaforos
    sem_init (&shared_data->sem1, 1, 1);
    sem_init (&shared_data->sem2, 1, 1);
    

    // Variables específicas del cocinero
    int capacidad_mesada = 27;
    


    // Ciclo de preparación de platos

while (shared_data-> platos_preparados < 180){

    while (shared_data->mesada < capacidad_mesada && shared_data-> platos_preparados < 180) {
       sem_wait(&shared_data->sem1);

            printf("Cocinero: Preparando platos...\n");
            shared_data->mesada += 1;
            shared_data-> platos_preparados += 1;
            printf("Capacidad mesada: %d\n", shared_data->mesada);
            printf("Cocinero: Platos preparados: %d\n", shared_data-> platos_preparados);
            sleep(1);
            
        sem_post(&shared_data->sem1);
        }
              
 }
   
    int valor=0;

     // Desvincular y liberar la memoria compartida
    shmdt(shared_data);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Se prepararon 180 platos, cerrando restaurante\n");

    return 0;

}