#include <sys/types.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <stdio.h>
int main() {
    struct utsname system_info;
  
    // Llamada a uname para obtener información del sistema
    if (uname(&system_info) == -1) {
        perror("Error al obtener información del sistema");
        return 1;
    }

    // Imprimir la versión del kernel
    printf("Versión del kernel: %s\n", system_info.release);
    printf("Versión del sistema operativo: %s %s\n", system_info.sysname, system_info.version);
       // Obtener y mostrar información del usuario actual
    printf("Nombre: Jonatan Leonel Garcia Arana \n");
    printf("202000424");

    return 0;
}