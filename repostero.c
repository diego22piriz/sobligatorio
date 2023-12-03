#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

typedef struct {
    int flanes_rellenados;
} SharedData;

int main() {
    // Obtener la memoria compartida existente
    int shmid = shmget(SHM_KEY, sizeof(SharedData), 0666);
    if (shmid == -1) {
        perror("Error al obtener la memoria compartida");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);

    // Variables
    int capacidad_heladera = 25;
    pid_t child_pid;

    // Ciclo de relleno de flanes
    while (shared_data->flanes_rellenados < 180) {
        // Verificar la capacidad de la heladera antes de rellenar más flanes
        if (shared_data->flanes_rellenados % capacidad_heladera == 0) {
            // Crear un proceso para rellenar más flanes
            child_pid = fork();

            if (child_pid == -1) {
                perror("Error al crear el proceso del repostero");
                exit(EXIT_FAILURE);
            }

            if (child_pid == 0) {
                // Código del repostero para rellenar flanes
                printf("Repostero: Rellenando flanes...\n");
                shared_data->flanes_rellenados += capacidad_heladera;
                printf("Repostero: Flanes rellenados: %d\n", shared_data->flanes_rellenados);
                exit(EXIT_SUCCESS);
            } else {
                // Esperar a que el hijo termine antes de continuar
                wait(NULL);
            }
        }
    }

    // Desvincular y liberar la memoria compartida
    shmdt(shared_data);

    printf("Repostero: Todos los flanes han sido rellenados. Cierre del programa.\n");

    return 0;
}


