#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


void hijo_escribir(int file_descriptor) {
  
    printf("proceso 1 ");
    char line[9];
    for (int i = 0; i < 8; ++i) {
        line[i] = 'a' + rand() % 26;
    }
    line[8] = '\0';
  
    write(file_descriptor, line, strlen(line));
    printf("Write: %s\n", line);
}


void hijo_leer(int file_descriptor) {
 
    char buffer[9];
    read(file_descriptor, buffer, 8);
    buffer[8] = '\0';
    printf("Read: %s\n", buffer);
}

void do_seek(int file_descriptor) {
 
    lseek(file_descriptor, 0, SEEK_SET);
    printf("Seek to the beginning of the file\n");
}

int main() {
     srand(time(NULL) + getpid()); // si no cambiamos aqui otro proceso hijo tendra el mismo seed y haran lo mismo ambos
         printf("inicia proceso 1");
    // Abre el archivo para lectura/escritura
    int file_descriptor = open("practica1.txt", O_RDWR | O_CREAT, 0644);
    if (file_descriptor == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }
    while (1) {
        int rand_call2 = rand() % 3; 
        sleep(rand() % 3 + 1);  //
        switch (rand_call2) {
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
