#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 4096

int main() {
    // Asignación de memoria compartida
    char *shared_memory = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // Crear un nuevo proceso

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // Proceso hijo
        // Leer desde la memoria compartida
        printf("Child reads: %s\n", shared_memory);
        // Liberar la memoria compartida
        munmap(shared_memory, SIZE);
        exit(EXIT_SUCCESS);
    } else { // Proceso padre
        // Escribir en la memoria compartida
        strcpy(shared_memory, "Hello, child process!");
        // Esperar que el proceso hijo termine
        wait(NULL);
        // Liberar la memoria compartida
        munmap(shared_memory, SIZE);
    }

    return 0;
}
