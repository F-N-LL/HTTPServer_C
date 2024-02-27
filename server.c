#include "server.h"
#include <stdio.h>
#include <stdlib.h>



struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void(*launch)(struct Server *server))
{
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

    server.socket = socket(domain, service, protocol); //El socket creado permite a nuestro SO  conectarse con la red por "dominio, servicio y protocolo"; Nos entrega una direccion de donde está ubicado el socket.

    if (server.socket == 0)
    {
        perror("Failed to connect the socket... \n");
        exit(1);
    }

    if ((bind(server.socket, (struct sockaddr *)&server.address, sizeof (server.address))) < 0)
   {
        perror("Failed to bind the socket.... \n");
        exit(1);
   }

    if ((listen(server.socket, server.backlog)) < 0)
    {
        perror("Failed to start listening...\n");
        exit(1);
    }

    server.launch = launch;

    
    return server;
}

