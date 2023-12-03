#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    // Número de flanes que puede preparar el repostero
    int total_flanes = 25;

    // Crear proceso para el repostero
    pid_t pid = fork();

    if (pid == 0) {
        // Código del repostero
        printf("Repostero: Rellenando la heladera con flanes...\n");
        sleep(3);  // Simulación de rellenar la heladera

        exit(0);   // Salir del proceso del repostero
    } else if (pid < 0) {
        perror("Error al crear el proceso del repostero");
        exit(EXIT_FAILURE);
    }

    // Esperar a que el repostero termine
    wait(NULL);

    printf("El repostero ha terminado. La heladera está lista.\n");

    return 0;
}

