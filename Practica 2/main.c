#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#define MAX_LINE_LENGTH 1000
#define MAX_USERS 150

struct data_struct
{
    int no_cuenta;
    char nombre[50];
    float saldo;
};

struct data_struct usuarios[MAX_USERS];
int contador_global = 0;
pthread_t thread1, thread2, thread3;

struct ThreadArgs
{
    FILE *file;
    int start_pos;
    int end_pos;
    int *contador_hilo_local; // Contador local para este hilo
};

int verificar_numero(int nuevo_numero, int contador_global)
{
    for (int i = 0; i < contador_global; i++)
    {
        if (nuevo_numero == usuarios[i].no_cuenta)
        {
            return 1; // El número de cuenta ya existe
        }
    }
    return 0; // El número de cuenta no existe
}
void *read_file(void *args)
{
    struct ThreadArgs *thread_args = (struct ThreadArgs *)args;
    FILE *file = thread_args->file;
    int start_pos = thread_args->start_pos;
    int end_pos = thread_args->end_pos;
    int *contador_hilo_local = thread_args->contador_hilo_local;

    char row[MAX_LINE_LENGTH];
    char *token;

    fseek(file, start_pos, SEEK_SET);
    fgets(row, MAX_LINE_LENGTH, file);

    while (ftell(file) < end_pos && fgets(row, MAX_LINE_LENGTH, file) != NULL && contador_global < MAX_USERS)
    {
        token = strtok(row, ",");
        int nuevo_numero_cuenta = atoi(token);

        if (verificar_numero(nuevo_numero_cuenta, contador_global))
        {
            printf("Error: Ya existe el numero de cuenta %d\n", contador_global + 2);
            (contador_global)++;
            (*contador_hilo_local)++;
        }
        else
        {
            usuarios[contador_global].no_cuenta = nuevo_numero_cuenta;
            token = strtok(NULL, ",");
            strcpy(usuarios[contador_global].nombre, token);
            token = strtok(NULL, ",");
            if (token == NULL)
            {
                printf("Error: Falta el saldo en la línea %d\n", contador_global + 2);
                usuarios[contador_global].saldo = 0.0;
            }
            else
            {
                float saldo_temp;
                if (sscanf(token, "%f", &saldo_temp) != 1 || saldo_temp < 0)
                {
                    printf("Error: Saldo no válido en la línea %d\n", contador_global + 2);
                    usuarios[contador_global].saldo = 0.0;
                }
                else
                {
                    usuarios[contador_global].saldo = saldo_temp;
                }
            }

            contador_global++;
            (*contador_hilo_local)++;
        }
    }

    pthread_exit(NULL);
}

int main()
{
    FILE *fp;
    fp = fopen("usuarios.csv", "r");
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    int tamano_archivo = ftell(fp);
    rewind(fp);

    int tamano_segmento = tamano_archivo / 3;

    int contador_hilo1 = 0;
    int contador_hilo2 = 0;
    int contador_hilo3 = 0;

    struct ThreadArgs args1 = {.file = fp, .start_pos = 0, .end_pos = tamano_segmento, .contador_hilo_local = &contador_hilo1};
    pthread_create(&thread1, NULL, read_file, &args1);
    pthread_join(thread1, NULL);
    struct ThreadArgs args2 = {.file = fp, .start_pos = tamano_segmento, .end_pos = tamano_segmento * 2, .contador_hilo_local = &contador_hilo2};
    pthread_create(&thread2, NULL, read_file, &args2);
    pthread_join(thread2, NULL);
    struct ThreadArgs args3 = {.file = fp, .start_pos = tamano_segmento * 2, .end_pos = tamano_archivo, .contador_hilo_local = &contador_hilo3};
    pthread_create(&thread3, NULL, read_file, &args3);

    pthread_join(thread3, NULL);

    printf("Usuarios leídos por hilo 1: %d\n", contador_hilo1);
    printf("Usuarios leídos por hilo 2: %d\n", contador_hilo2);
    printf("Usuarios leídos por hilo 3: %d\n", contador_hilo3);

    contador_global = contador_hilo1 + contador_hilo2 + contador_hilo3;

    for (int i = 0; i < contador_global; i++)
    {
        printf("No_cuenta: %d  Nombre: %s   Saldo: %.2f\n",
               usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }

    fclose(fp);
    return 0;
}
