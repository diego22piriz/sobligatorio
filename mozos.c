#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    // Número de mesas y capacidad de la mesada
    int num_mesas = 33;
    int capacidad_mesada = 27;

    // Crear procesos para los mozos
    for (int i = 0; i < num_mesas; ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            // Código del mozo
            printf("Mozo %d: Retirando platos de la mesada para la mesa %d...\n", i + 1, i + 1);
            sleep(1);  // Simulación de llevar los platos a la mesa

            exit(0);   // Salir del proceso del mozo
        } else if (pid < 0) {
            perror("Error al crear el proceso del mozo");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que todos los mozos terminen
    for (int i = 0; i < num_mesas; ++i) {
        wait(NULL);
    }

    printf("Todos los mozos han terminado. La cantina se cierra.\n");

    return 0;
}
