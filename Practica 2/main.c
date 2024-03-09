#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_LINE_LENGTH 1000
#define MAX_USERS 100

// Definición de la estructura para los datos de usuario
typedef struct {
    int id;
    char first_name[50];
    char last_name[50];
    char email[100];
} Usuario;

// Estructura para pasar argumentos a la función de carga de usuarios
typedef struct {
    FILE *f;
    Usuario *usuarios;
    int *contador;
    int inicio;
    int fin;
} ArgsCargarUsuarios;

// Función para cargar usuarios desde un archivo CSV
void *cargar_usuarios(void *args) {
    ArgsCargarUsuarios *argumentos = (ArgsCargarUsuarios *)args;
    FILE *f = argumentos->f;
    Usuario *usuarios = argumentos->usuarios;
    int *contador = argumentos->contador;
    int inicio = argumentos->inicio;
    int fin = argumentos->fin;

    char row[MAX_LINE_LENGTH];
    char *token;

    fseek(f, 0, SEEK_SET); // Establecer el cursor al inicio del archivo

    // Leer líneas hasta llegar a la línea de inicio asignada al hilo
    for (int i = 0; i < inicio; i++) {
        fgets(row, MAX_LINE_LENGTH, f);
    }

    // Leer y procesar las líneas asignadas al hilo
    int n = 0;
    while (fgets(row, MAX_LINE_LENGTH, f) != NULL && n < fin - inicio) {
        token = strtok(row, ",");
        usuarios[*contador].id = atoi(token);

        token = strtok(NULL, ",");
        sprintf(usuarios[*contador].first_name, "%s", token);

        token = strtok(NULL, ",");
        sprintf(usuarios[*contador].last_name, "%s", token);

        token = strtok(NULL, ",");
        sprintf(usuarios[*contador].email, "%s", token);

        (*contador)++;
        n++;
    }

    pthread_exit(NULL);
}

int main() {
    FILE *f = fopen("usuarios.csv", "r");
    if (f == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    Usuario usuarios[MAX_USERS];
    int contador_total = 0;

    // Calcular la cantidad de usuarios en el archivo
    char row[MAX_LINE_LENGTH];
    int num_usuarios = 0;
    while (fgets(row, MAX_LINE_LENGTH, f) != NULL) {
        num_usuarios++;
    }
    rewind(f); // Volver al principio del archivo

    // Definir la cantidad de hilos y la cantidad de usuarios que cada hilo procesará
    int num_hilos = 3;
    int usuarios_por_hilo = num_usuarios / num_hilos;

    // Iniciar los hilos para cargar usuarios
    pthread_t hilos[num_hilos];
    ArgsCargarUsuarios args[num_hilos];

    for (int i = 0; i < num_hilos; i++) {
        args[i].f = f;
        args[i].usuarios = usuarios;
        args[i].contador = &contador_total;
        args[i].inicio = i * usuarios_por_hilo;
        args[i].fin = (i + 1) * usuarios_por_hilo;

        if (i == num_hilos - 1) {
            // El último hilo procesa los usuarios restantes
            args[i].fin = num_usuarios;
        }

        if (pthread_create(&hilos[i], NULL, cargar_usuarios, (void *)&args[i]) != 0) {
            perror("Error al crear el hilo");
            exit(EXIT_FAILURE);
        }
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < num_hilos; i++) {
        if (pthread_join(hilos[i], NULL) != 0) {
            perror("Error al unirse al hilo");
            exit(EXIT_FAILURE);
        }
    }

    // Imprimir los usuarios cargados
    for (int i = 0; i < contador_total; i++) {
        printf("Id: %d   Nombre: %s   Apellido: %s   Email: %s\n", 
            usuarios[i].id, usuarios[i].first_name, usuarios[i].last_name, usuarios[i].email);
    }

    fclose(f);
    return 0;
}
