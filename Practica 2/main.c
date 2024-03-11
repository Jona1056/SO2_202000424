#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_LINE_LENGTH 1000
#define MAX_USERS 150
//commmit
struct data_struct {
    int no_cuenta;
    char nombre[50];
    float saldo; // Cambiado a float para admitir decimales
};

struct data_struct usuarios[MAX_USERS];
int contador_global = 0; // Contador de usuarios leídos
int contador_hilo1 = 0;
int contador_hilo2 = 0;
int contador_hilo3 = 0;
pthread_t thread1, thread2, thread3; 
// Estructura para pasar argumentos a la función de lectura de archivos
struct ThreadArgs {
    FILE *file;
    int start_pos;
    int end_pos;
};

// Función para que el hilo lea su parte del archivo
void *read_file(void *args) {
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
    FILE *file = thread_args->file;
    int start_pos = thread_args->start_pos;
    int end_pos = thread_args->end_pos;
    int contador_hilo_local = 0; // Contador local para este hilo

    char row[MAX_LINE_LENGTH];
    char *token;

    fseek(file, start_pos, SEEK_SET);
    fgets(row, MAX_LINE_LENGTH, file);

    while (ftell(file) < end_pos && fgets(row, MAX_LINE_LENGTH, file) != NULL && contador_global < MAX_USERS) {
        token = strtok(row, ",");
        usuarios[contador_global].no_cuenta = atoi(token);

        token = strtok(NULL, ",");
        strcpy(usuarios[contador_global].nombre, token);

        token = strtok(NULL, ",");
        if (sscanf(token, "%f", &usuarios[contador_global].saldo) != 1) {
            printf("Error: Saldo no válido en la línea %d\n", contador_global + 2);
            usuarios[contador_global].saldo = 0.0; // Valor predeterminado en caso de error
        }

        contador_global++;  
         contador_hilo_local++;
    }
     if (pthread_equal(pthread_self(), thread1)) {
        contador_hilo1 = contador_hilo_local;
    } if (pthread_equal(pthread_self(), thread2)) {
        contador_hilo2 = contador_hilo_local;
    }  if (pthread_equal(pthread_self(), thread3)) {
        contador_hilo3 = contador_hilo_local;
    }

    pthread_exit(NULL);
}

int main() {
    FILE *fp;
    fp = fopen("usuarios.csv", "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    int tamano_archivo = ftell(fp);
    rewind(fp);

    int tamano_segmento = tamano_archivo / 3;

    struct ThreadArgs args = { .file = fp, .start_pos = 0, .end_pos = tamano_segmento };
    pthread_create(&thread1, NULL, read_file, &args);
    pthread_join(thread1, NULL);

 
    struct ThreadArgs args2 = { .file = fp, .start_pos = tamano_segmento, .end_pos =  tamano_segmento * 2};
    pthread_create(&thread2, NULL, read_file, &args2);
    pthread_join(thread2, NULL);

    struct ThreadArgs args3 = { .file = fp, .start_pos = tamano_segmento * 2, .end_pos =  tamano_archivo};
    pthread_create(&thread3, NULL, read_file, &args3);
    pthread_join(thread3, NULL);

     printf("Usuarios leídos por hilo 1: %d\n", contador_hilo1);
    printf("Usuarios leídos por hilo 2: %d\n", contador_hilo2);
    printf("Usuarios leídos por hilo 3: %d\n", contador_hilo3);
    //crear otro hilo que lea desde la posicion donde se quedo el anterior
    for (int i = 0; i < contador_global; i++) {
        printf("No_cuenta: %d  Nombre: %s   Saldo: %.2f\n", 
               usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }

    fclose(fp);
    return 0;
}