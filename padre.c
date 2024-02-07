#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void main(){

    pid_t pid = fork(); //Se realiza el fork
    
    if(pid == -1){
        perror("fork");
        exit(1);
    }
    printf("El valor de pid es: %d\n", pid);
    
    if(pid == 0){
        printf("Soy el proceso hijo. Mi PID es: %d\n", getpid());

        /*Se escriben los argumentos para el proceso hijo*/
        char *arg_Ptr[4];
       
        arg_Ptr[0] = " niño.c";
        arg_Ptr[1] = " Hola! ";
        arg_Ptr[2] = " Soy el proceso hijo! ";
        arg_Ptr[3] = NULL; // El ultimo indice de argv siempre debe de ser NULL

        /*Se ejecuta el ejecutable del proceso hijo*/
        execv("/home/oem/Documentos/prueba c++/niño.bin", arg_Ptr);
    
    }else{
        printf("Soy el proceso padre. El PID de mi hijo es: %d\n", pid);
     

        /*Se espera a que el proceso hijo termine*/
        int status;
        wait(&status);

        /*Se imprime el codigo de salida del proceso hijo*/
        if(WIFEXITED(status)){
            printf("\nEl proceso hijo termino con estado: %d\n", WIFEXITED(status));
        } else {
            printf("Ocurrio un error al terminar el proceso hijo");
        }

        printf("Terminando proceso padre\n");
    }
}