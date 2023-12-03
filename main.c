#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // Crear un segmento de memoria compartida
    size_t size = sizeof(int);
    int *shared_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if (shared_data == MAP_FAILED) {
        perror("Error al crear la memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Inicializar la memoria compartida (podrías almacenar datos compartidos aquí)

    // Ejecutar los programas desde la línea de comandos
    system("./cocineros &");
    system("./mozos &");
    system("./repostero &");

    // Esperar a que todos los programas terminen
    wait(NULL);

    // Liberar la memoria compartida
    munmap(shared_data, size);

    return 0;
}
