## **CREANDO UN SERVIDOR HTTP PASO A PASO EN C**


## Servidor HTTP simple en C

**Descripción:**

Este README describe un servidor HTTP básico implementado en lenguaje C. El servidor puede escuchar conexiones entrantes en un puerto específico y responder a solicitudes HTTP con un mensaje simple.

**Requisitos:**

* Compilador de C (gcc)

**Instrucciones de uso:**

1. **Compilación:**

```
gcc -o server server.c -lstdc++
```

2. **Ejecución:**

```
./server
```

3. **Acceso al servidor:**

Abra un navegador web e ingrese la siguiente dirección en la barra de direcciones:

```
http://localhost:8080
```

**Explicación del código:**

El código fuente del servidor se encuentra en el archivo `server.c`. El código define una estructura `Server` que contiene la información necesaria para configurar el servidor, como el puerto de escucha, la dirección IP y la función que se ejecutará cuando se inicie el servidor.

La función `main` del programa crea una instancia de la estructura `Server` y configura los parámetros necesarios. Luego, llama a la función `server_constructor` para inicializar el servidor y comenzar a escuchar conexiones entrantes.

Cuando se recibe una conexión entrante, el servidor crea un nuevo hilo para atender la solicitud del cliente. El hilo lee la solicitud del cliente y envía una respuesta simple que contiene un mensaje HTML.


## **INSTRUCCIONES PARA EL DESARROLLO DEL SERVIDOR**


## **`server.h:` Cabecera del programa**

**1. Protección de inclusiones múltiples:**

```c
#ifndef Server_h
#define Server_h
```

Estas líneas evitan que el archivo `server.h` se incluya más de una vez en tu código. Esto ayuda a prevenir errores de compilación y asegura que las definiciones sólo se realicen una vez.

**2. Inclusion de librerías:**

```c
#include <sys/socket.h>
#include <netinet/in.h>
```

Estas líneas incluyen las librerías necesarias para trabajar con sockets de red y estructuras de direcciones de internet.

**3. Estructura Server:**

```c
struct Server
{
   int domain;
   int service;
   int protocol;
   int port;
   u_long interface;
   int backlog;

   struct sockaddr_in address; //cuidado con sockaddr_in6 IPv6
   
   int socket;

   void (*launch)(struct Server *server);
};
```

Esta estructura define un "Servidor" con los siguientes campos:

- `domain`: Dominio del socket (por ejemplo, `AF_INET` para IPv4).
- `service`: Tipo de servicio (por ejemplo, `SOCK_STREAM` para TCP o `SOCK_DGRAM` para UDP).
- `protocol`: Protocolo de transporte (por ejemplo, 0 para el protocolo predeterminado).
- `port`: Puerto en el que escuchará el servidor.
- `interface`: Interfaz de red en la que escuchar (por ejemplo, `INADDR_ANY` para escuchar en todas las interfaces).
- `backlog`: Número máximo de conexiones pendientes en la cola de escucha del servidor.
- `address`: Estructura `sockaddr_in` que almacena la dirección del servidor (dirección IP y puerto).
- `socket`: Descriptor del socket creado.
- `launch`: Puntero a una función que se ejecutará cuando se inicie el servidor. Este campo permite especificar una función que se llamará cuando se cree el servidor para realizar cualquier configuración o inicialización adicional.

**4. Función constructora server_constructor:**

```c
struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void(*launch)(struct Server *server));
```

Esta función crea una instancia de la estructura `Server` con los parámetros especificados y devuelve la estructura inicializada. La función toma siete argumentos que corresponden a los campos de la estructura `Server` y devuelve una instancia inicializada de la estructura.

## **`server.c:` Archivo principal**

**1. Librerias:**

`#**include** "server.h"#**include** <stdio.h>#**include** <stdlib.h>`

Se incluyen las librerías necesarias:

- `server.h`: El archivo de cabecera que define la estructura `Server` y la función `server_constructor`.
- `<stdio.h>`: La librería estándar de entrada y salida que proporciona funciones para la entrada y salida estándar (por ejemplo, `printf`, `scanf`).
- `<stdlib.h>`: La librería estándar de utilidades generales que proporciona diversas funciones como `exit`.

**2. Implementación de server_constructor:**

```c
struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void(*launch)(struct Server *server))
{
    struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.port = port;
    server.interface = interface;
    server.backlog = backlog;

}
```

Esta sección implementa la función `server_constructor` que ya se definió en el archivo `server.h`. La función crea una instancia de la estructura `Server`, asigna los valores de los parámetros recibidos a los campos correspondientes de la estructura, configura la dirección del servidor, crea el socket, verifica la creación del socket y configura el socket para escuchar.

**3. Creación de la estructura Server:**

```c
struct Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.port = port;
    server.interface = interface;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(domain, service, protocol);
```

Primero se declara una variable `server` de tipo `Server`. Luego, se asignan los valores de los parámetros recibidos a los campos correspondientes de la estructura.

**4. Configuración de la dirección del servidor:**

```c
server.address.sin_family = domain;
server.address.sin_port = htons(port);
server.address.sin_addr.s_addr = htonl(interface);
```

- Se configura la familia de direcciones a la indicada por el parámetro `domain`.
- Se convierte el número de puerto a formato de red usando la función `htons`.
- Se convierte la dirección IP a

**5. Creación del socket:**

```c
server.socket = socket(domain, service, protocol);

if (server.socket == 0)
{
    perror("Failed to connect the socket... \n");
    exit(1);
}
```

Se crea el socket usando la función `socket`. La función toma tres argumentos:

- `domain`: Familia de direcciones del socket.
- `service`: Tipo de servicio del socket.
- `protocol`: Protocolo de transporte del socket.

Si la creación del socket falla, se imprime un mensaje de error y se termina el programa.

**6. Enlazar el socket a una dirección:**

```c
if ((bind(server.socket, (struct sockaddr *)&server.address, sizeof (server.address))) < 0)
{
    perror("Failed to bind the socket.... \n");
    exit(1);
}
```

Se utiliza la función `bind` para enlazar el socket a la dirección especificada en la estructura `server.address`. La función toma tres argumentos:

- `socket`: Descriptor del socket.
- `address`: Puntero a la estructura `sockaddr` que contiene la dirección del socket.
- `length`: Longitud de la estructura `sockaddr`.

Si la función `bind` falla, se imprime un mensaje de error y se termina el programa.

**7. Poner el socket en modo escucha:**

```c
if ((listen(server.socket, server.backlog)) < 0)
{
    perror("Failed to start listening...\n");
    exit(1);
}
```

Se utiliza la función `listen` para poner el socket en modo escucha. La función toma dos argumentos:

- `socket`: Descriptor del socket.
- `backlog`: Número máximo de conexiones pendientes en la cola de escucha.

Si la función `listen` falla, se imprime un mensaje de error y se termina el programa.

**8. Almacenar la función de inicio:**

```c
server.launch = launch;
```

Se guarda la función que se ejecutará cuando se inicie el servidor en el campo `launch` de la estructura `server`.

**9. Devolver la estructura Server:**

```c
return server;
```

Se devuelve la estructura `server` con los campos configurados.

## **`test.c`: Probando el server**

El código en `test.c` crea un servidor y lo inicia. La función `launch` se encarga de esperar a que se conecten clientes, leer la solicitud del cliente y enviar una respuesta. La respuesta enviada es un mensaje HTML simple que dice "Hola equipo de XXX".

**test.c:**

```c
#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void launch(struct Server *server)
{
    char buffer[40000];
    char *hello = "<html><body><h1>Hola equipo de XXX</h1></body></html>\n";
    int address_lenght = sizeof(server->address);
    int new_socket;

     while(1)
    {
        printf("===== ESPERANDO A CONECTAR =====\n");
        int address_lenght = sizeof(server->address);
        new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_lenght);
        read(new_socket, buffer, 40000);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));
        close(new_socket);
    }
}

int main () {

    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, launch);
    server.launch(&server); 
    return 0;

}
```

- `#include "server.h"`: Esta línea incluye el archivo `server.h` que define la estructura `Server` y la función `server_constructor` .
- `#include <stdio.h>`: Esta línea incluye la librería `stdio.h` que proporciona funciones para la entrada y salida estándar.
- `#include <string.h>`: Esta línea incluye la librería `string.h` que proporciona funciones para trabajar con cadenas de caracteres.
- `#include <unistd.h>`: Esta línea incluye la librería `unistd.h` que proporciona diversas funciones como `read` y `write`.
- `void launch(struct Server *server)`: Esta línea define la función `launch` que se ejecutará cuando se inicie el servidor.
- `char buffer[40000]`: Esta línea declara una variable `buffer` de tipo `char` con un tamaño de 40000 bytes.
- `char *hello = "<html><body><h1>Hola equipo de XXX</h1></body></html>\n"`: Esta línea define una variable `hello` de tipo `char` que contiene un mensaje HTML simple.
- `int address_lenght = sizeof(server->address)`: Esta línea define una variable `address_lenght` que almacena el tamaño de la estructura `address` dentro de la estructura `server`.
- `int new_socket`: Esta línea define una variable `new_socket` que almacena el descriptor del socket del nuevo cliente.
- `while(1)`: Esta línea crea un bucle infinito que se ejecutará mientras el servidor esté funcionando.
- `printf("===== ESPERANDO A CONECTAR =====\n")`: Esta línea imprime un mensaje que indica que el servidor está esperando a que se conecten clientes.
- `new_socket = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t *)&address_lenght)`: Esta línea acepta una conexión entrante y el descriptor del socket del nuevo cliente se asigna a la variable `new_socket`.
- `read(new_socket, buffer, 40000)`: Esta línea lee la solicitud del cliente y la guarda en el buffer.
- `printf("%s\n", buffer)`: Esta línea imprime la solicitud del cliente.
- `write(new_socket, hello, strlen(hello))`: Esta línea envía la respuesta al cliente.
- `close(new_socket)`: Esta línea cierra el socket del nuevo cliente.

## **Explicación detallada de las decisiones tomadas en el código:**

**1. Familia de direcciones AF_INET:**

Se ha utilizado la familia de direcciones `AF_INET` para IPv4. Esta es la familia de direcciones más utilizada para redes IP. La principal razón para elegirla es que es compatible con la gran mayoría de los dispositivos y aplicaciones en internet.

**2. Tipo de servicio SOCK_STREAM:**

Se ha utilizado el tipo de servicio `SOCK_STREAM` para TCP. Este tipo de servicio proporciona una conexión fiable entre dos hosts, lo que significa que los datos se envían en orden y sin errores. Es la opción ideal para aplicaciones como servidores web, correo electrónico y transferencia de archivos.

**3. Protocolo de transporte 0:**

Se ha utilizado el protocolo de transporte 0 para el protocolo predeterminado. Esto indica que se utilizará el protocolo TCP por defecto. En la mayoría de los casos, no es necesario especificar el protocolo de transporte, ya que se seleccionará automáticamente el protocolo adecuado.

**4. Función htons:**

Se ha utilizado la función `htons` para convertir el número de puerto a formato de red. Los ordenadores utilizan diferentes formatos para almacenar los números, por lo que es necesario convertir el número de puerto al formato de red antes de usarlo. La función `htons` realiza esta conversión para los sistemas que utilizan ordenadores de bytes grandes (big-endian).

**5. Función htonl:**

Se ha utilizado la función `htonl` para convertir la dirección IP a formato de red. Al igual que con el número de puerto, es necesario convertir la dirección IP al formato de red antes de usarla. La función `htonl` realiza esta conversión para los sistemas que utilizan ordenadores de bytes grandes (big-endian).

**6. Función accept:**

Se ha utilizado la función `accept` para aceptar conexiones entrantes. Esta función permite al servidor esperar a que un cliente se conecte y aceptar la conexión. Una vez que se acepta la conexión, el servidor puede empezar a comunicarse con el cliente.

**7. Función read:**

Se ha utilizado la función `read` para leer la solicitud del cliente. Esta función permite al servidor leer datos del socket del cliente. La solicitud del cliente contiene información sobre lo que el cliente desea del servidor.

**8. Función write:**

Se ha utilizado la función `write` para enviar la respuesta al cliente. Esta función permite al servidor enviar datos al socket del cliente. La respuesta del servidor contiene la información que el cliente ha solicitado.

**9. Función close:**

Se ha utilizado la función `close` para cerrar el socket del cliente. Esta función libera los recursos asociados al socket del cliente. Es importante cerrar el socket del cliente cuando ya no se necesita para evitar fugas de memoria.

### **Justificación de las decisiones:**

Las decisiones tomadas en el código se basan en las siguientes razones:

- **Compatibilidad:** Se han elegido opciones compatibles con la gran mayoría de dispositivos y aplicaciones en internet.
- **Fiabilidad:** Se han elegido opciones que proporcionan una conexión fiable entre el servidor y el cliente.
- **Eficiencia:** Se han elegido opciones que son eficientes en el uso de recursos.
- **Simplicidad:** Se han elegido opciones que son fáciles de entender y usar.

**En resumen, las decisiones tomadas en el código están orientadas a crear un servidor HTTP simple, eficiente y compatible con la mayoría de dispositivos y aplicaciones en internet.**

**Puntos a tener en cuenta:**

- Este es un ejemplo básico de un servidor HTTP. Un servidor HTTP completo necesitaría implementar la lógica para procesar solicitudes HTTP y enviar respuestas.
- El código no maneja errores de forma completa. Se deberían agregar controladores de errores para manejar errores de red, errores de socket y otros errores que puedan ocurrir.
