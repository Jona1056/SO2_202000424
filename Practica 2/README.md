<<<<<<< HEAD
# Manual Tecnico
## Jonatan Leonel Garcia Arana
## Introduccion 
El presente documento describe la practica destinada a comprender el funcionamiento de las
llamadas al sistema en el contexto del sistema operativo Linux MINT. Se abordarán temas
relacionados con la gestión de procesos, así como la capacidad de interceptar estas llamadas
para monitorear y registrar su actividad.
El objetivo principal es desarrollar un programa en lenguaje C que actúe como proceso padre y
cree dos procesos hijos. Estos procesos hijos realizarán operaciones de manejo de archivos
sobre un archivo específico, mientras que el proceso padre interceptará y registrará las llamadas
al sistema realizadas por los hijos.
#
## Requerimientos minimos
#
## Requerimientos para Linux Mint:
- Sistema operativo: Linux Mint (versión específica, por ejemplo, Linux Mint 20.3 "Una").
- Procesador: Procesador compatible con arquitectura de 32 bits (x86) o 64 bits (x64)
- Memoria RAM: Se recomienda al menos 1 GB de RAM para compilaciones simples y
desarrollo básico en C. Para proyectos más complejos, se recomiendan 2 GB o más.
- Espacio en disco: Se recomienda un mínimo de 16 GB de espacio disponible en disco
- Herramientas de desarrollo: Compilador de C compatible con Linux Mint (se puede
instalar GCC, por ejemplo)
# 
## Instalación y Configuración de Herramientas de Desarrollo:
- Compilador GCC: El compilador GCC generalmente viene preinstalado en la mayoría de
las distribuciones de Linux, incluyendo Linux Mint. Si no está instalado, puede instalarlo
a través del gestor de paquetes de su distribución con el siguiente comando:
sudo apt-get install build-essential
- Editor de texto o IDE: Puede utilizar cualquier editor de texto de su preferencia, como
Vim, Emacs, o instalar un IDE como NetBeans, Code::Blocks, o Visual Studio Code
#
## INICIANDO EJECUCION DEL PROGRAMA
#
Para esta practica estaremos utilizando 3 archivos los cuales son los siguientes:
- Padre.c: se encargara de la creacion de los procesos hijos y de la llamada a systemtrap
- Hijo.c: tendra la logica necesaria para simular los procesos para cuando el padre mande a llamar a los hijos
- Trace.stp: archivo de systemtrap, en el cual estaremos monitereando todas las llamdas a los procesos hijos(Read,Write,Seek)
### Archivo Padre:
#### librerias utilizadas 
- stdio.h
- stdlib.h
- unistd.h
- syst/wait.h
- string.h
- signal.h
#### Variables
#
<br>
<img src="./Imagenes/variables.png" alt="drawing"/>

### INICIO DEL CODIGO

En esta parte creamos unos contadores, los cuales nos ayudaran a llevar el control delos PID y asimismo declaramos el metodo signal, el cual nos ayuda a capturar el ctrl + C, mas adelante explicaremos que hace cuando se hace esto.
<br>
<img src="./Imagenes/Inicio_padre.png" alt="drawing"/>
<br>
Luego declaramos el primer fork, que es el primero que estaremos utilizando en el programa
<br>
<img src="./Imagenes/logica_fork.png" alt="drawing" />

en esta parte del codigo, tendremos la logica para ejecutar nuestro procesos hijos, el cual si entra al primer if que iguala a -1, este es un error, luego sabemos que si el PID que viene es mayor a 0 es el proceso padre, encontes cuando entra a este else if, creamos el segundo proceso hijo y siguiendo la misma logica para el primer hijo pues verificamos si el PID2 que viene es padre o hijo y si no es mayor a 0 es el proceso hijo, entonces en el else del proceso hijo, mandamos a ejecutar el archivo hijo.bin, esto lo hacemos con execv. 
ambos hijos llaman al mismo archivo, ya que esatermos simulando el mismo proceso
#
### llamada a archivo systemtrap
para la llamada de este archivo tenemos un char de 100 caracteres, el cual utilizaremos para almacenar el comando, luego con ayuda de un snprintf, que guarda el comando, el cual seria sudo stap trace.stp, el cual va redirigir toda su salida al archivo syscall.log. Este archivo trace.stp va recibir dos parametros, el cual es el pid del hijo 1 en este caso contador1 y el pid del hijo 2 en este caso contador 2, con el system(command) mandamos a ejecutar el systemrap y luego con un waitpid esperamos a que terminen los procesos hijos.
<br>
<img src="./Imagenes/llamada_systemtrap.png" alt="drawing" />
### Método hijo que escribe en el archivo: 
en esta parte, tenemos un metodo llamdo hijo_escribir, el cual recibe el file, que seria el open del archivo practica1.txt, se crea un char de 8 caracteres y con un for, el cual tiene un random, ingresa a escribir números o a escribir letras, luego con un write escribimos en el archivo
Area de trabajo de Recursos y Configuracion IP
<br>
<img src="./Imagenes/hijo_escribir.png" alt="drawing">

#
### Metodo hijo que lee archivo:
En este metodo tenemos un char el cual nos ayuda a leer las 8 posiciones del archivo, esto donde se encuentre el apuntador.
<br>
<img src="./Imagenes/hijo_leer.png" alt="drawing" />

#
### Metodo seek, que redirecciona a la primera posicion:
Con un lseek podemos redireccionar a cualquier lugar del archivo el apuntador, en este caso lo redireccionamos a la posicion 0
<br>
<img src="./Imagenes/hijo_seek.png" alt="drawing" />

#
### Metodo que captura la señal ctrl + C:
En esta parte del codigo tendermos la logica para obtener el conteo de los procesos que se hicieron, para esto utilizamos un fopen, el cual abre el archivo syscall.logs y recorremos el archivo con un while, en el cual si encuentra la palabra read, write o seek, suma la variable dependiendo la condicion encontrada, con una serie de pasos simples, obtenemos el conteo toal y ya imprimimos en consola nuestros procesos totales.
<br>
<img src="./Imagenes/signint.png" alt="drawing" />

#
### Funcion main del archivo hijo
En esta funcion, creamos el archivo practica1.txt en el cual se hace los procesos de escribir, leer y redireccionar el puntero, entonces con un while hacemos que se repita el c´digo, luego con un random de 1 a 3, ponemos un sleep para que espere un tiempo aleatorio y con un switch y un random, elije que funcion manda a llamar, que en este caso seria hijo_escribir, hijo_leer e hijo_seek, esto se repitira hasta que se termine el proceso hijo.
<br>
<img src="./Imagenes/main_hijo.png" alt="drawing" />

#
### Metodos en el archivo de systemtrap:
Systemrap es una herramientas que nos ayuda a llevar el control de procesos en Linux, entonces con systemtrap podemos crear un script el cual nos ayudara a saber que procesos se hacen con el respectivo PID
### Metodo que controla los read:
Systemtrap nos da la opcion de controlar las llamadas que se hacen, entonces con un probe syscall.read, podemos obtener todos los read, pero para buscar los read que hacen los PID delos hijos tenemos que poner un if, en el cual igualando con los parametros que obtiene el comando, podemos encontrar el PID al cual pertenece, como un tipo de filtro.
<br>
<img src="./Imagenes/systemtrap_read.png" alt="drawing" />
<br>
### Metodo que controla los write:
De igual manera que el método de read, lo único que cambia con este método es
el syscall.read, en vez de ser read, pues utilizaríamos la palabra write, la cual
buscaría todos los procesos write, que concuerden con el pid de los procesos hijos
<br>
<img src="./Imagenes/systemtrap_write.png" alt="drawing" />
<br>
### Metodo que controla los seek:
Como con los métodos anteriores, lo único que cambieremos será el nombre del
método, que en este caso seria syscall.lseek, el cual leerá todos los seek que
hagamos cuando concuerde los pid de los procesos hijos
<br>
<img src="./Imagenes/systemtrap_seek.png" alt="drawing" />



=======
# Manual Tecnico
## Jonatan Leonel Garcia Arana
## Introduccion 
El presente documento describe la practica destinada a comprender el funcionamiento de las
llamadas al sistema en el contexto del sistema operativo Linux MINT. Se abordarán temas
relacionados con la gestión de procesos, así como la capacidad de interceptar estas llamadas
para monitorear y registrar su actividad.
El objetivo principal es desarrollar un programa en lenguaje C que actúe como proceso padre y
cree dos procesos hijos. Estos procesos hijos realizarán operaciones de manejo de archivos
sobre un archivo específico, mientras que el proceso padre interceptará y registrará las llamadas
al sistema realizadas por los hijos.
#
## Requerimientos minimos
#
## Requerimientos para Linux Mint:
- Sistema operativo: Linux Mint (versión específica, por ejemplo, Linux Mint 20.3 "Una").
- Procesador: Procesador compatible con arquitectura de 32 bits (x86) o 64 bits (x64)
- Memoria RAM: Se recomienda al menos 1 GB de RAM para compilaciones simples y
desarrollo básico en C. Para proyectos más complejos, se recomiendan 2 GB o más.
- Espacio en disco: Se recomienda un mínimo de 16 GB de espacio disponible en disco
- Herramientas de desarrollo: Compilador de C compatible con Linux Mint (se puede
instalar GCC, por ejemplo)
# 
## Instalación y Configuración de Herramientas de Desarrollo:
- Compilador GCC: El compilador GCC generalmente viene preinstalado en la mayoría de
las distribuciones de Linux, incluyendo Linux Mint. Si no está instalado, puede instalarlo
a través del gestor de paquetes de su distribución con el siguiente comando:
sudo apt-get install build-essential
- Editor de texto o IDE: Puede utilizar cualquier editor de texto de su preferencia, como
Vim, Emacs, o instalar un IDE como NetBeans, Code::Blocks, o Visual Studio Code
#
## INICIANDO EJECUCION DEL PROGRAMA
#
Para esta practica estaremos utilizando 3 archivos los cuales son los siguientes:
- Padre.c: se encargara de la creacion de los procesos hijos y de la llamada a systemtrap
- Hijo.c: tendra la logica necesaria para simular los procesos para cuando el padre mande a llamar a los hijos
- Trace.stp: archivo de systemtrap, en el cual estaremos monitereando todas las llamdas a los procesos hijos(Read,Write,Seek)
### Archivo Padre:
#### librerias utilizadas 
- stdio.h
- stdlib.h
- unistd.h
- syst/wait.h
- string.h
- signal.h
#### Variables
#
<br>
<img src="./Imagenes/variables.png" alt="drawing"/>

### INICIO DEL CODIGO

En esta parte creamos unos contadores, los cuales nos ayudaran a llevar el control delos PID y asimismo declaramos el metodo signal, el cual nos ayuda a capturar el ctrl + C, mas adelante explicaremos que hace cuando se hace esto.
<br>
<img src="./Imagenes/Inicio_padre.png" alt="drawing"/>
<br>
Luego declaramos el primer fork, que es el primero que estaremos utilizando en el programa
<br>
<img src="./Imagenes/logica_fork.png" alt="drawing" />

en esta parte del codigo, tendremos la logica para ejecutar nuestro procesos hijos, el cual si entra al primer if que iguala a -1, este es un error, luego sabemos que si el PID que viene es mayor a 0 es el proceso padre, encontes cuando entra a este else if, creamos el segundo proceso hijo y siguiendo la misma logica para el primer hijo pues verificamos si el PID2 que viene es padre o hijo y si no es mayor a 0 es el proceso hijo, entonces en el else del proceso hijo, mandamos a ejecutar el archivo hijo.bin, esto lo hacemos con execv. 
ambos hijos llaman al mismo archivo, ya que esatermos simulando el mismo proceso
#
### llamada a archivo systemtrap
para la llamada de este archivo tenemos un char de 100 caracteres, el cual utilizaremos para almacenar el comando, luego con ayuda de un snprintf, que guarda el comando, el cual seria sudo stap trace.stp, el cual va redirigir toda su salida al archivo syscall.log. Este archivo trace.stp va recibir dos parametros, el cual es el pid del hijo 1 en este caso contador1 y el pid del hijo 2 en este caso contador 2, con el system(command) mandamos a ejecutar el systemrap y luego con un waitpid esperamos a que terminen los procesos hijos.
<br>
<img src="./Imagenes/llamada_systemtrap.png" alt="drawing" />
### Método hijo que escribe en el archivo: 
en esta parte, tenemos un metodo llamdo hijo_escribir, el cual recibe el file, que seria el open del archivo practica1.txt, se crea un char de 8 caracteres y con un for, el cual tiene un random, ingresa a escribir números o a escribir letras, luego con un write escribimos en el archivo
Area de trabajo de Recursos y Configuracion IP
<br>
<img src="./Imagenes/hijo_escribir.png" alt="drawing">

#
### Metodo hijo que lee archivo:
En este metodo tenemos un char el cual nos ayuda a leer las 8 posiciones del archivo, esto donde se encuentre el apuntador.
<br>
<img src="./Imagenes/hijo_leer.png" alt="drawing" />

#
### Metodo seek, que redirecciona a la primera posicion:
Con un lseek podemos redireccionar a cualquier lugar del archivo el apuntador, en este caso lo redireccionamos a la posicion 0
<br>
<img src="./Imagenes/hijo_seek.png" alt="drawing" />

#
### Metodo que captura la señal ctrl + C:
En esta parte del codigo tendermos la logica para obtener el conteo de los procesos que se hicieron, para esto utilizamos un fopen, el cual abre el archivo syscall.logs y recorremos el archivo con un while, en el cual si encuentra la palabra read, write o seek, suma la variable dependiendo la condicion encontrada, con una serie de pasos simples, obtenemos el conteo toal y ya imprimimos en consola nuestros procesos totales.
<br>
<img src="./Imagenes/signint.png" alt="drawing" />

#
### Funcion main del archivo hijo
En esta funcion, creamos el archivo practica1.txt en el cual se hace los procesos de escribir, leer y redireccionar el puntero, entonces con un while hacemos que se repita el c´digo, luego con un random de 1 a 3, ponemos un sleep para que espere un tiempo aleatorio y con un switch y un random, elije que funcion manda a llamar, que en este caso seria hijo_escribir, hijo_leer e hijo_seek, esto se repitira hasta que se termine el proceso hijo.
<br>
<img src="./Imagenes/main_hijo.png" alt="drawing" />

#
### Metodos en el archivo de systemtrap:
Systemrap es una herramientas que nos ayuda a llevar el control de procesos en Linux, entonces con systemtrap podemos crear un script el cual nos ayudara a saber que procesos se hacen con el respectivo PID
### Metodo que controla los read:
Systemtrap nos da la opcion de controlar las llamadas que se hacen, entonces con un probe syscall.read, podemos obtener todos los read, pero para buscar los read que hacen los PID delos hijos tenemos que poner un if, en el cual igualando con los parametros que obtiene el comando, podemos encontrar el PID al cual pertenece, como un tipo de filtro.
<br>
<img src="./Imagenes/systemtrap_read.png" alt="drawing" />
<br>
### Metodo que controla los write:
De igual manera que el método de read, lo único que cambia con este método es
el syscall.read, en vez de ser read, pues utilizaríamos la palabra write, la cual
buscaría todos los procesos write, que concuerden con el pid de los procesos hijos
<br>
<img src="./Imagenes/systemtrap_write.png" alt="drawing" />
<br>
### Metodo que controla los seek:
Como con los métodos anteriores, lo único que cambieremos será el nombre del
método, que en este caso seria syscall.lseek, el cual leerá todos los seek que
hagamos cuando concuerde los pid de los procesos hijos
<br>
<img src="./Imagenes/systemtrap_seek.png" alt="drawing" />



>>>>>>> 9127f191b6f5dfba24816d8a45badfc1ac6cdf8d
