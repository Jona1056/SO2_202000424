#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define MAX_BUFFER_SIZE 1024
#define SYSLOG_FILE "syscalls.log"
int total_calls = 0;
int read_calls = 0;
int write_calls = 0;
int seek_calls = 0;

void sigint_handler(int signum) {
    
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp_calls = fopen(SYSLOG_FILE, "r");
    if (fp_calls == NULL) {
        perror("Error al abrir el archivo de llamadas");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp_calls) != NULL) {
        if (strstr(buffer, "read") != NULL) {
            read_calls++;
        } else if (strstr(buffer, "write") != NULL) {
            write_calls++;
        }else if (strstr(buffer, "seek") != NULL){
            seek_calls++;
        }
    }

    // Cerrar el archivo después de leerlo
    fclose(fp_calls);

    // Calcular el total de llamadas
    total_calls = read_calls + write_calls + seek_calls;

    // Imprimir resultados
    printf("Número total de llamadas al sistema: %d\n", total_calls);
    printf("Número de llamadas al sistema por tipo:\n");
    printf("Read: %d\n", read_calls);
    printf("Write: %d\n", write_calls);
    printf("Seek: %d\n", seek_calls);
    // Terminar la ejecución del programa
    exit(EXIT_SUCCESS);
}
int main() {
    int contador1 = 0;
    int contador2 = 0;
    signal(SIGINT, sigint_handler);
    pid_t pid2 = 0;
    FILE *fp;
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error al hacer fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Estamos en el proceso padre
        //printf("Proceso hpadre creado con PID: %d\n", getpid());
        contador1 = pid; // Guardamos el PID del primer hijo
        
        pid_t pid2 = fork(); // Creamos el segundo proceso hijo
        if (pid2 == -1) {
            perror("Error al hacer fork");
            exit(EXIT_FAILURE);
        } else if (pid2 > 0) {
            // Estamos en el proceso padre
            contador2 = pid2; // Guardamos el PID del segundo hijo
            char command[100];
            snprintf(command, sizeof(command), "sudo stap trace.stp %d %d > syscalls.log &", contador1, contador2);
            system(command);
            int status;
            waitpid(pid, &status, 0);
            waitpid(pid2, &status, 0);
        } else {
            // Estamos en el proceso hijo 2
            //printf("Proceso hijo 2 creado con PID: %d\n", getpid());
            char *args[] = {"0", NULL};
            execv("/home/oem/Desktop/SO2_202000424/Practica 1/hijo.bin", args);
            perror("Error en execv");
            exit(EXIT_FAILURE);
        }
    } else {
        // Estamos en el proceso hijo 1
        //printf("Proceso hijo 1 creado con PID: %d\n", getpid());
        char *args[] = {"0", NULL};
        execv("/home/oem/Desktop/SO2_202000424/Practica 1/hijo.bin", args);
        perror("Error en execv");
        exit(EXIT_FAILURE);
    }

    // Código que se ejecuta solo en el proceso padre
    //printf("PID del primer hijo: %d\n", contador1);
    //printf("PID del segundo hijo: %d\n", contador2);
    
    // Esperar a que ambos hijos terminen
    
    

    // Cerrar el archivo
    fclose(fp);

   
    
    return 0;
}
