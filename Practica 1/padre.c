#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

// Contador para el número total de llamadas al sistema
int total_syscalls = 0;

// Manejador de señal para SIGINT (Ctrl + C)
void sigint_handler(int sig) {
    printf("\nNúmero total de llamadas al sistema por los procesos hijo durante la ejecución: %d\n", total_syscalls);
    exit(EXIT_SUCCESS);
}

int main() {
    // Establecer el manejador de señales para SIGINT
    signal(SIGINT, sigint_handler);

    // Vaciar o crear el archivo de registro "syscalls.log"
    int log_fd = open("syscalls.log", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (log_fd == -1) {
        perror("Error al abrir el archivo de log");
        exit(EXIT_FAILURE);
    }

    // Crear dos procesos hijos
    for (int i = 0; i < 2; i++) {
        pid_t pid = fork();
      
        if (pid == -1) {
            perror("Error al hacer fork");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
        
            // Redirigir stdout y stderr al archivo de registro "syscalls.log"
            //ver que hijo esta ejecutando
            if(i==0){
                printf("Hijo 1\n");
                //imprmir su pid
                printf("PID: %d\n", getpid());
                char *args[] = {"//home/oem/Documentos/SO2_202000424/Practica 1/hijo.bin", NULL};
            execv("/home/oem/Documentos/SO2_202000424/Practica 1/hijo.bin", args);
            }
            else if(i==1){
                printf("Hijo 2\n");
                //imprmir su pid
                printf("PID: %d\n", getpid());
                char *args[] = {"/home/oem/Documentos/SO2_202000424/Practica 1/hijo2.bin", NULL};
                execv("/home/oem/Documentos/SO2_202000424/Practica 1/hijo2.bin", args);
        
            }

            // Ejecutar el programa hijo
           
            perror("Error en execv");
            exit(EXIT_FAILURE);
        }
    }

    // Cerrar el archivo de log en el proceso padre
    close(log_fd);

    // Esperar a que los hijos terminen
    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            // Incrementar el contador de llamadas al sistema
            
        }
    }

    return 0;
}
