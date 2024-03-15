#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>
#define MAX_LINE_LENGTH 1000
#define MAX_USERS 150
#define MAX_DATE_LENGTH 20
//variables

pthread_t thread1, thread2, thread3; //hilos
#define MAX_ERRORES 1000
char errores[MAX_ERRORES][MAX_LINE_LENGTH]; 
int num_errores = 0; //errroes
int contador_global = 0; //contadores
int usuarios_en_sistema = 0;
int contador_hilo1 = 0;
int contador_hilo2 = 0;
int contador_hilo3 = 0;

struct data_struct
{
    int no_cuenta;
    char nombre[50];
    float saldo;
};
struct data_struct usuarios[MAX_USERS];

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
            return 1;
        }
    }
    return 0;
}

char *obtenerfecha()
{
    time_t now;
    static char fecha[MAX_DATE_LENGTH]; // Almacenar la fecha en una variable estática para que persista fuera de la función
    struct tm *local_time;
    time(&now);
    local_time = localtime(&now);
    strftime(fecha, MAX_DATE_LENGTH, "%Y-%m-%d %H:%M:%S", local_time);
    return fecha;
}

void eliminar_salto_linea(char *cadena)
{
    size_t longitud = strlen(cadena);
    if (longitud > 0 && cadena[longitud - 1] == '\n')
    {
        cadena[longitud - 1] = '\0'; // Reemplaza el salto de línea con el carácter nulo
    }
}
int reporte_usuarios()
{
    char *fecha_actual = obtenerfecha();
    char archivo_usuarios[50];
    sprintf(archivo_usuarios,"%s.log",fecha_actual);
    FILE *archivo_errores = fopen(archivo_usuarios,"w");
    if (archivo_errores == NULL)
    {
        perror("Error al abrir el archivo de errores");
        return 1;
    }
    fprintf(archivo_errores, "%s\n", "------- Carga de Usuarios ------");
  
    fprintf(archivo_errores, "%s\n", "");
    fprintf(archivo_errores, "Fecha: %s\n", fecha_actual);
    fprintf(archivo_errores, "%s\n", "");
    fprintf(archivo_errores, "%s\n", "Usuarios Cargados:");
    fprintf(archivo_errores, "Hilo#1: %d\n", contador_hilo1);
    fprintf(archivo_errores, "Hilo#2: %d\n", contador_hilo2);
    fprintf(archivo_errores, "Hilo#3: %d\n", contador_hilo3);
    fprintf(archivo_errores, "Total: %d\n", contador_hilo1+contador_hilo2+contador_hilo3);
    fprintf(archivo_errores, "%s\n", "");

    // Escribir los errores en el archivo
    fprintf(archivo_errores, "%s\n", "Errores: ");
    for (int i = 0; i < num_errores; i++)
    {
        eliminar_salto_linea(errores[i]);
        fprintf(archivo_errores, "- %s\n", errores[i]);
    }
    fclose(archivo_errores);
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
            sprintf(errores[num_errores++], "Linea %d: Ya existe el numero de cuenta %d", contador_global + 2, nuevo_numero_cuenta);
           
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
                int contiene_letras = 0;
                char token_string[MAX_LINE_LENGTH];
                strcpy(token_string, token);
                for (int i = 0; token[i] != '\0'; i++)
                {
                    if (!isdigit(token[i]) && token[i] != '.' && token[i] != '-' && token[i] != '\n')
                    {
                        contiene_letras = 1;
                        break;
                    }
                }
                if (contiene_letras)
                {
                    eliminar_salto_linea(token);
                    sprintf(errores[num_errores++], "Linea %d: El saldo '%s' contiene caracteres no válidos", contador_global + 2, token);
                    usuarios[contador_global].saldo = 0.0;
                }
                else
                {
                    float saldo_temp;
                    if (sscanf(token, "%f", &saldo_temp) != 1 || saldo_temp < 0)
                    {
                        eliminar_salto_linea(token);
                        sprintf(errores[num_errores++], "Linea %d: El saldo es negativo '%s' ", contador_global + 2, token);
                        usuarios[contador_global].saldo = 0.0;
                    }
                    else
                    {
                        usuarios[contador_global].saldo = saldo_temp;
                        contador_global++;
                        (*contador_hilo_local)++;
                    }
                }
            }

    
        }
    }
    pthread_exit(NULL);
}

void cargar_usuarios(const char *ruta_archivo){
    FILE *fp;
    fp = fopen(ruta_archivo, "r");
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0, SEEK_END);
    int tamano_archivo = ftell(fp);
    rewind(fp);

    int tamano_segmento = tamano_archivo / 3;

    struct ThreadArgs args1 = {.file = fp, .start_pos = 0, .end_pos = tamano_segmento, .contador_hilo_local = &contador_hilo1};
    pthread_create(&thread1, NULL, read_file, &args1);
    pthread_join(thread1, NULL);
    struct ThreadArgs args2 = {.file = fp, .start_pos = tamano_segmento, .end_pos = tamano_segmento * 2, .contador_hilo_local = &contador_hilo2};
    pthread_create(&thread2, NULL, read_file, &args2);
    pthread_join(thread2, NULL);
    struct ThreadArgs args3 = {.file = fp, .start_pos = tamano_segmento * 2, .end_pos = tamano_archivo, .contador_hilo_local = &contador_hilo3};
    pthread_create(&thread3, NULL, read_file, &args3);
    pthread_join(thread3, NULL);

    printf("%s\n","Usuarios Cargados Correctamente");
 
    
    reporte_usuarios();
    contador_hilo1 = 0;
    contador_hilo2 = 0;
    contador_hilo3 = 0;
    fclose(fp);

}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
int verificar_cuenta(const int cuenta,int *posicion){
    for (int i = 0; i < contador_global; i++)
    {
        if(cuenta == usuarios[i].no_cuenta){
            *posicion = i;
            return 1;
        }
    }
    return 0;
}

int verificar_monto(const  double monto) {
    int numero = monto;
    int punto_decimal = 0; // Variable para contar los puntos decimales encontrados
    while (numero != 0) {
        int digito = numero % 10;
        if (!isdigit(digito + '0') && digito != '.') {
            return 0; // El monto no es válido
        }
       
        numero /= 10;
    }
    return 1; // El monto es válido
}

int deposito(const int cuenta, const double monto,const int validacion){
    int posicion;
    if(!verificar_cuenta(cuenta,&posicion)){
        printf( "\n   La cuenta no existe\n");
        return 1;
    }
    if(!verificar_monto(monto)){
        printf( "\n   El monto es invalido\n");
        return 1;
    }
    usuarios[posicion].saldo = usuarios[posicion].saldo + monto;
    printf( "\n   Deposito correcto");
    return 0;
}
int retiro(const int cuenta, const double monto,const int validacion){
    int posicion;
    if(!verificar_cuenta(cuenta,&posicion)){
        printf( "\n   La cuenta no existe\n");
        return 1;
    }
    if(!verificar_monto(monto)){
        printf( "\n   El monto es invalido\n");
        return 1;
    }
    if(monto > usuarios[posicion].saldo){
        printf( "\n   No cuenta con suficiente saldo\n");
        return 1;
    }

    usuarios[posicion].saldo = usuarios[posicion].saldo - monto;
    printf( "\n   Debito correcto");
    return 0;
}
int consultar_cuenta(const int cuenta){
    int posicion;
    if(!verificar_cuenta(cuenta,&posicion)){
        printf( "\n   La cuenta no existe\n");
        return 1;
    }
    printf("\n   DATOS:  ");
    printf("\n   Numero de cuenta: %d",usuarios[posicion].no_cuenta);
    printf("\n   Nombre: %s",usuarios[posicion].nombre);
    printf("\n   Monto Total: %f",usuarios[posicion].saldo);
    printf("\n");

    return 0;
}

int estados_cuenta(){
   
    FILE *archivo_estados= fopen("estados_cuenta.csv","w");
    if (archivo_estados== NULL)
    {
        perror("Error al abrir el archivo de errores");
        return 1;
    }
    fprintf(archivo_estados, "no_cuenta,nombre,saldo\n");
    for (int i = 0; i < contador_global; i++) {
        fprintf(archivo_estados, "%d,%s,%.2lf\n", usuarios[i].no_cuenta, usuarios[i].nombre, usuarios[i].saldo);
    }
    fclose(archivo_estados);
    printf("Datos de usuarios escritos en estados_cuenta.csv correctamente.\n");

}


int main()
{
    
    int opcion;
    char ruta_archivo[1000];
    int cuenta1;
    int cuenta2;
    double monto;
    
    do
    {
        
        printf( "\n   1. Deposito");
        printf( "\n   2. Retiro" );
        printf( "\n   3. Transaccion");
        printf( "\n   4. Consultar Cuenta" );
        printf( "\n   5. Cargar Operaciones" );
        printf( "\n   6. Cargar Usuarios" );
        printf( "\n   7. Estados de Cuentas" );
        printf( "\n   8. Salir." );
        printf( "\n\n   Introduzca opcion (1-8): " );


        if (scanf("%d", &opcion) != 1) {
            printf("\nError: Ingrese un número entero válido.\n");
            limpiar_buffer(); // Limpia el buffer de entrada
            continue;
        }
        switch ( opcion )
        {
            case 1: printf( "\n   Deposito");
                    printf( "\n   Numero de cuenta: " );
                    scanf("%d", &cuenta1);
                    printf( "   Monto a Transferir: " );
                    scanf("%lf", &monto);
                    deposito(cuenta1,monto,0);
                    break;

            case 2: printf( "\n  Retiro");
                    printf( "\n  Numero de cuenta: " );
                    scanf("%d", &cuenta1);
                    printf( "  Monto a debitar: " );
                    scanf("%lf", &monto);
                    retiro(cuenta1,monto,0);
                    break;
                    break;

            case 3: printf( "\n   Introduzca un n%cmero entero: ", 163 );
                   
            case 4: printf( "\n   Consultar Cuenta");
                    printf( "\n   Numero de cuenta: " );
                    scanf("%d", &cuenta1);
                    consultar_cuenta(cuenta1);
                    break;
                  
            case 6:
                    printf("Ingrese la ruta del archivo de usuarios: ");
                    limpiar_buffer(); // Limpia el buffer de entrada antes de leer la cadena
                    fgets(ruta_archivo, sizeof(ruta_archivo), stdin);
                    ruta_archivo[strcspn(ruta_archivo, "\n")] = '\0'; // Elimina el carácter de nueva línea
                    cargar_usuarios(ruta_archivo);
                    break;
            case 7:
                    printf("\n   Estados de cuenta");
                    printf("\n ");
                    estados_cuenta();
                    break;
            default:
                    printf( "\n  Opcion Invalida");
                    break;
         }

    
    } while ( opcion != 8 );
    return 0;
}
