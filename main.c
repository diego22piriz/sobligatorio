#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

typedef struct {
    int platos_preparados;
    int platos_retirados;
    int flanes_rellenados;
} SharedData;

void runCocineros() {
    execlp("./cocineros", "cocineros", NULL);
}

void runMozos() {
    execlp("./mozos", "mozos", NULL);
}

void runRepostero() {
    execlp("./repostero", "repostero", NULL);
}

int main() {
    // Crear e inicializar la memoria compartida
    int shmid = shmget(SHM_KEY, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Error al crear la memoria compartida");
        exit(EXIT_FAILURE);
    }

    SharedData *shared_data = (SharedData *)shmat(shmid, NULL, 0);
    shared_data->platos_preparados = 0;
    shared_data->platos_retirados = 0;
    shared_data->flanes_rellenados = 0;

    // Crear procesos para los cocineros, mozos y repostero
    pid_t cocineros_pid, mozos_pid, repostero_pid;

    cocineros_pid = fork();
    if (cocineros_pid == 0) {
        // Código para los cocineros
        runCocineros();
        exit(EXIT_SUCCESS);
    }

    mozos_pid = fork();
    if (mozos_pid == 0) {
        // Código para los mozos
        runMozos();
        exit(EXIT_SUCCESS);
    }

    repostero_pid = fork();
    if (repostero_pid == 0) {
        // Código para el repostero
        runRepostero();
        exit(EXIT_SUCCESS);
    }

    // Esperar a que todos los procesos hijos finalicen
    waitpid(cocineros_pid, NULL, 0);
    waitpid(mozos_pid, NULL, 0);
    waitpid(repostero_pid, NULL, 0);

    // Desvincular y liberar la memoria compartida
    shmdt(shared_data);
    shmctl(shmid, IPC_RMID, NULL);

    printf("Programa principal: Todos los procesos han finalizado. Cierre del programa.\n");

    return 0;
}
