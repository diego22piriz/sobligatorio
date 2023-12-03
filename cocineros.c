#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    // Número de platos que pueden preparar los cocineros
    int total_platos = 180;

    // Número de cocineros
    int num_cocineros = 3;

    // Crear procesos para los cocineros
    for (int i = 0; i < num_cocineros; ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            // Código del cocinero
            printf("Cocinero %d: Preparando platos...\n", i + 1);
            sleep(2);  // Simulación de preparación de platos

            exit(0);   // Salir del proceso del cocinero
        } else if (pid < 0) {
            perror("Error al crear el proceso del cocinero");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que todos los cocineros terminen
    for (int i = 0; i < num_cocineros; ++i) {
        wait(NULL);
    }

    printf("Todos los cocineros han terminado. La cocina se cierra.\n");

    return 0;
}
