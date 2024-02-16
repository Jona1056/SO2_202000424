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

void sigint_handler(int signum) {
    char buffer[MAX_BUFFER_SIZE];
    FILE *fp_calls, *fp_syscalls;
    fp_syscalls = fopen(SYSLOG_FILE, "w");
    if (fp_syscalls == NULL) {
        perror("Error al abrir o vaciar el archivo de syscalls");
        exit(EXIT_FAILURE);
    }
    fclose(fp_syscalls);
    fp_calls = fopen("calls.log", "r");
    if (fp_calls == NULL) {
        perror("Error al abrir el archivo calls.log");
        exit(EXIT_FAILURE);
    }
    fp_syscalls = fopen(SYSLOG_FILE, "a");
    if (fp_syscalls == NULL) {
        perror("Error al abrir el archivo de syscalls para escritura");
        fclose(fp_calls);
        exit(EXIT_FAILURE);
    }

    // Leer el archivo calls.log línea por línea y obtener los recuentos de las llamadas
    while (fgets(buffer, sizeof(buffer), fp_calls) != NULL) {
        if (strstr(buffer, "READ_COUNTER=") != NULL) {
            read_calls++;
            sscanf(buffer, "READ_COUNTER=%d", &read_calls);
        } else if (strstr(buffer, "WRITE_COUNTER=") != NULL) {
            write_calls++;
            sscanf(buffer, "WRITE_COUNTER=%d", &write_calls);
        } else {
            fputs(buffer, fp_syscalls);
            // Imprimir las llamadas al sistema en la consola
            
        }
    }
    // Imprimir el número total de llamadas al sistema
    total_calls = read_calls + write_calls;
    printf("Número total de llamadas al sistema: %d\n", total_calls);

    // Imprimir el número de llamadas al sistema por tipo
    printf("Número de llamadas al sistema por tipo:\n");
    printf("Read: %d\n", read_calls);
    printf("Write: %d\n", write_calls);

    // Terminar la ejecución del programa
    exit(EXIT_SUCCESS);
}
int main() {
    int contador1 = 0;
    int contador2 = 0;
    pid_t pid = fork();
    pid_t pid2 = 0;
    
    FILE *fp;
    char buffer[MAX_BUFFER_SIZE];

    signal(SIGINT, sigint_handler);
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
        } else {
            // Estamos en el proceso hijo 2
            //printf("Proceso hijo 2 creado con PID: %d\n", getpid());
            char *args[] = {"/home/oem/Desktop/SO2_202000424/Practica 1/hijo.bin", NULL};
            execv("/home/oem/Desktop/SO2_202000424/Practica 1/hijo.bin", args);
            perror("Error en execv");
            exit(EXIT_FAILURE);
        }
    } else {
        // Estamos en el proceso hijo 1
        //printf("Proceso hijo 1 creado con PID: %d\n", getpid());
        char *args[] = {"/home/oem/Desktop/SO2_202000424/Practica 1/hijo.bin", NULL};
        execv("/home/oem/Desktop/SO2_202000424/Practica 1/hijo.bin", args);
        perror("Error en execv");
        exit(EXIT_FAILURE);
    }

    // Código que se ejecuta solo en el proceso padre
    //printf("PID del primer hijo: %d\n", contador1);
    //printf("PID del segundo hijo: %d\n", contador2);
    
    // Esperar a que ambos hijos terminen
    char command[100];
    snprintf(command, sizeof(command), "sudo stap trace.stp %d %d > calls.log &", contador1, contador2);
    system(command);
    int status;
    waitpid(pid, &status, 0);
    waitpid(pid2, &status, 0);
    

    // Cerrar el archivo
    fclose(fp);

   
    
    return 0;
}
