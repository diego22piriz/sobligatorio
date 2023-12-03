#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

typedef struct {
    int platos_retirados;
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
    int capacidad_bandeja = 4;
    pid_t child_pid;

    // Ciclo de retiro de platos
    while (shared_data->platos_retirados < 180) {
        // Crear un proceso para retirar platos
        child_pid = fork();

        if (child_pid == -1) {
            perror("Error al crear el proceso del mozo");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            // Código del mozo para retirar platos
            printf("Mozo: Retirando platos...\n");
            shared_data->platos_retirados += capacidad_bandeja;
            printf("Mozo: Platos retirados: %d\n", shared_data->platos_retirados);
            exit(EXIT_SUCCESS);
        } else {
            // Esperar a que el hijo termine antes de continuar
            wait(NULL);
        }
    }

    // Desvincular la memoria compartida
    shmdt(shared_data);

    printf("Mozo: Todos los platos han sido retirados. Cierre del programa.\n");

    return 0;
}
