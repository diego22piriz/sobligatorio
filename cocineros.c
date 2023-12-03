#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

typedef struct {
    int platos_preparados;
} SharedData;

int main() {
    // Crear e inicializar la memoria compartida
    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error al crear la memoria compartida");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    shared_data->platos_preparados = 0;

    // Variables
    int total_platos = 180;
    int capacidad_mesada = 27;
    pid_t child_pid;

    // Ciclo de preparación de platos
    while (shared_data->platos_preparados < total_platos) {
        // Verificar la capacidad de la mesada antes de preparar más platos
        if (shared_data->platos_preparados % capacidad_mesada == 0) {
            // Crear un proceso para preparar más platos
            child_pid = fork();

            if (child_pid == -1) {
                perror("Error al crear el proceso del cocinero");
                exit(EXIT_FAILURE);
            }

            if (child_pid == 0) {
                // Código del cocinero para preparar platos
                printf("Cocinero: Preparando platos...\n");
                shared_data->platos_preparados += capacidad_mesada;
                printf("Cocinero: Platos preparados: %d\n", shared_data->platos_preparados);
                exit(EXIT_SUCCESS);
            } else {
                // Esperar a que el hijo termine antes de continuar
                wait(NULL);
            }
        }
    }

    // Desvincular y liberar la memoria compartida
    shmdt(shared_data);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Cocinero: Todos los platos han sido preparados. Cierre del programa.\n");

    return 0;
}


