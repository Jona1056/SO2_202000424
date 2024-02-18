# Manual Tecnico
## Jonatan Leonel Garcia Arana
# Objetivos 
- Proporcionar una descripción completa de la topología de la red diseñada para el pequeño negocio de dos niveles, incluyendo la disposición de los equipos, la configuración de los switches, y la estructura de la red en estrella.
- Detallar la configuración de las máquinas virtuales VPC y switches de capa 2, así como la implementación de los protocolos Ethernet, IP, ARP e ICMP en la red diseñada.
- Proporcionar una guía detallada y paso a paso sobre cómo recrear la topología de red en Packet Tracer, incluyendo la configuración de dispositivos, la asignación de direcciones IP, y la interconexión de equipos.
#
# Requerimientos minimos
#
Cisco Packet Tracer 8.2 (64 bits):
- Computadora con uno de los siguientes sistemas operativos: Microsoft Windows 8.1, 10, 11 (64 bits), Ubuntu 20.04, 22.04 LTS (64 bits) o macOS 10.14 o posterior.
- CPU amd64 (x86-64)
- 4 GB de RAM libre
- 1,4 GB de espacio en disco libre
#
Cisco Packet Tracer 8.2 (32 bits):
- Computadora con uno de los siguientes sistemas operativos: Microsoft Windows 8.1, 10 (32 bits)
- CPU x86 compatible
- 2 GB de RAM libre
- 1,4 GB de espacio en disco libre
#
# Configuracion de las VPCs
#
Area de trabajo de la Oficina A y Configuracion IP
<br>
<img src="./Imagenes/Conf_oficinaA.png" alt="drawing" />
<img src="./Imagenes/OficinaA.png" />
#
Area de trabajo de la Oficina B y Configuracion IP
<br>
<img src="./Imagenes/Conf_oficinaB.png" alt="drawing"/>
<img src="./Imagenes/OficinaB.png"  />

#
Area de trabajo de la Oficina C y Configuracion IP
<br>
<img src="./Imagenes/Conf_oficinaC.png" alt="drawing"/>
<img src="./Imagenes/OficinaC.png" alt="drawing" />

#
Area de trabajo de Gerencia y Configuracion IP
<br>
<img src="./Imagenes/Conf_gerencia.png" alt="drawing" />
<img src="./Imagenes/Gerencia.png" alt="drawing"/>

#
Area de trabajo de Administracion y Configuracion IP
<br>
<img src="./Imagenes/Conf_administracion.png" alt="drawing" />
<img src="./Imagenes/Administracion.png" alt="drawing" />

#
Area de trabajo de Recursos y Configuracion IP
<br>
<img src="./Imagenes/Conf_recursos.png" alt="drawing">
<img src="./Imagenes/Recursos.png" alt="drawing" />
#
Area de trabajo de Atencion al Cliente y Configuracion IP
<br>
<img src="./Imagenes/Conf_atencionalcliente.png" alt="drawing" />
<img src="./Imagenes/Atencionalcliente.png" alt="drawing" />
#
# COMUNICACION ENTRE AREAS
Comunicacion desde Atencion al cliente a oficina B e IP de la oficina B
<br>
<img src="./Imagenes/Atencion_OficinaB.png" alt="drawing" />
<img src="./Imagenes/ping_oficinaB.png" alt="drawing" />
#
Comunicacion desde Oficina A a recursos humanos e IP de recursos
<br>
<img src="./Imagenes/OficinaA_recursos.png" alt="drawing" />
<img src="./Imagenes/ping_recursos.png" alt="drawing" />
#
Comunicacion desde Gerencia a oficina C e IP de la oficina C
<br>
<img src="./Imagenes/Gerencia_OficinaC.png" alt="drawing" />
<img src="./Imagenes/ping_oficinaC.png" alt="drawing" />

# Captura de paquete ARP
ip a donde mandamos el paquete
<br>
<img src="./Imagenes/ip_paquete.png" alt="drawing" />
<br>
<img src="./Imagenes/ping_a_paquete.png" alt="drawing" />
<br>
<img src="./Imagenes/PING_ARP.png" alt="drawing" />
<img src="./Imagenes/paquete_enviado.png" alt="drawing" />


#

# Conclusion
La realización de esta práctica nos permitio demostrar la capacidad para diseñar y configurar una red local pequeña utilizando la herramienta Packet Tracer. Al finalizar la práctica, hemos adquirido experiencia en la creación de topologías de red en estrella, la configuración de switches, la asignación de direcciones IP y la realización de capturas de paquetes para verificar la conectividad entre los dispositivos.


