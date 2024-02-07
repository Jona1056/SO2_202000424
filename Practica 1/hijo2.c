#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

// Función para realizar una llamada de sistema Write
void hijo_escribir(int file_descriptor) {
    // Genera una cadena de 8 caracteres aleatorios
    printf("proceso 2 ");
    char line[9];
    for (int i = 0; i < 8; ++i) {
        line[i] = 'a' + rand() % 26;
    }
    line[8] = '\0';
    // Escribe la cadena en el archivo
    write(file_descriptor, line, strlen(line));
    printf("Write: %s\n", line);
}

// Función para realizar una llamada de sistema Read
void hijo_leer(int file_descriptor) {
    // Lee 8 caracteres del archivo
    char buffer[9];
    read(file_descriptor, buffer, 8);
    buffer[8] = '\0';
    printf("Read: %s\n", buffer);
}

// Función para realizar una llamada de sistema Seek
void do_seek(int file_descriptor) {
    // Reposiciona el puntero de lectura/escritura al principio del archivo
    lseek(file_descriptor, 0, SEEK_SET);
    printf("Seek to the beginning of the file\n");
}

int main() {
    srand(time(NULL));
    printf("inicia proceso 2");

    // Abre el archivo para lectura/escritura
    int file_descriptor = open("practica1.txt", O_RDWR | O_CREAT, 0644);
    if (file_descriptor == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }
    while (1) {
        int rand_call = rand() % 3; // Genera un número aleatorio entre 0 y 2
        sleep(rand() % 3 + 1); // Espera entre 1 y 3 segundos antes de la siguiente llamada
        switch (rand_call) {
            case 0:
                hijo_escribir(file_descriptor);
                break;
            case 1:
                hijo_leer(file_descriptor);
                break;
            case 2:
                hijo_seek(file_descriptor);
                break;
        }
    }
    // Cierra el archivo
    close(file_descriptor);
    return 0;
}
