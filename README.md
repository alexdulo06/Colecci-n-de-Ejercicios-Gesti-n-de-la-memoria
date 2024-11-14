# Colecci-n-de-Ejercicios-Gesti-n-de-la-memoria
https://github.com/alexdulo06/Colecci-n-de-Ejercicios-Gesti-n-de-la-memoria.git

#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 4096

int main() {
    char *shared_memory = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child reads: %s\n", shared_memory);
        munmap(shared_memory, SIZE);
        exit(EXIT_SUCCESS);
    } else {
        strcpy(shared_memory, "Hello, child process!");
        wait(NULL);
        munmap(shared_memory, SIZE);
    }

    return 0;
}
Explicación paso a paso:

Primero, importamos las bibliotecas necesarias para la asignación de memoria y la creación de procesos.

Creamos una constante SIZE para determinar el tamaño de la región de memoria compartida que queremos asignar.

En la función principal, llamamos a la función mmap() para asignar una región de memoria que pueda ser compartida entre procesos. Los argumentos de esta función son los siguientes:

NULL: Le estamos pidiendo al sistema operativo que elija la dirección de inicio de la región de memoria.
SIZE: Es el tamaño de la región que queremos asignar.
PROT_READ|PROT_WRITE: Estamos indicando que la región de memoria puede ser leída y escrita.
MAP_SHARED|MAP_ANONYMOUS: Estas banderas indican que la región de memoria será compartida entre procesos y que no estará respaldada por ningún archivo.
-1: Este argumento sería el descriptor de archivo si la memoria estuviera respaldada por un archivo, pero como usamos MAP_ANONYMOUS, podemos poner -1.
0: Este sería el desplazamiento si la memoria estuviera respaldada por un archivo. Como no es el caso, ponemos 0.
Comprobamos si la asignación fue exitosa. Si mmap() devuelve MAP_FAILED, significa que hubo un error.

Creamos un nuevo proceso utilizando la función fork(). Esta función devuelve el PID del proceso hijo al proceso padre y 0 al proceso hijo.

Luego, comprobamos el valor devuelto por fork(). Si es menor que 0, hubo un error al crear el nuevo proceso.

Si el valor devuelto es 0, significa que estamos en el proceso hijo. En este caso, leemos y mostramos la memoria compartida y luego liberamos la memoria utilizando la función munmap().

Si el valor devuelto por fork() es mayor que 0, significa que estamos en el proceso padre. En este caso, escribimos en la memoria compartida, esperamos a que el proceso hijo termine y luego liberamos la memoria.

Espero que esta explicación te ayude a entender mejor el código. Si tienes más preguntas, no dudes en hacerlas.

**descripcion paso a paso**
Descripción general del proyecto:
El objetivo de este proyecto es demostrar cómo se puede usar la memoria compartida para la comunicación entre procesos en un sistema operativo tipo Unix (como Linux). Utiliza las funciones mmap(), fork(), y wait() para crear un proceso hijo, compartir información entre el padre y el hijo a través de memoria compartida, y manejar la sincronización entre los procesos.

Paso 1: Asignación de memoria compartida
El programa comienza solicitando una región de memoria compartida.
Para ello, usa la función mmap() que asigna un bloque de memoria que puede ser accedido tanto por el proceso padre como por el proceso hijo.
La memoria es asignada con permisos de lectura y escritura, y se configura como compartida, lo que significa que los cambios hechos en la memoria por uno de los procesos serán visibles para el otro.
Paso 2: Creación de un proceso hijo
A continuación, el programa crea un nuevo proceso hijo utilizando la función fork().
Si fork() es exitoso, el proceso se divide en dos:
El proceso padre continúa su ejecución con el código que sigue después de fork().
El proceso hijo comienza desde el mismo punto donde ocurrió fork().
Paso 3: Comunicación entre procesos
El proceso padre escribe un mensaje en la región de memoria compartida. Este mensaje será accesible por el proceso hijo.
El proceso hijo, al leer la memoria compartida, puede acceder al mensaje que el proceso padre escribió y luego mostrarlo en la consola.
Esta es la parte clave del proyecto: la memoria compartida permite que ambos procesos interactúen y se comuniquen sin necesidad de mecanismos de comunicación más complejos (como pipes o sockets).
Paso 4: Sincronización entre procesos
Para asegurar que el proceso hijo lea la memoria después de que el proceso padre haya escrito el mensaje, el proceso padre usa wait() para esperar a que el hijo termine su tarea antes de proceder.
Esto garantiza que el proceso padre no termine ni libere la memoria compartida antes de que el proceso hijo haya terminado de leer el mensaje.
Paso 5: Liberación de recursos
Después de que el proceso hijo lee el mensaje, ambos procesos deben liberar la memoria compartida utilizando la función munmap().
Esto asegura que los recursos del sistema sean liberados correctamente y no haya fugas de memoria.
