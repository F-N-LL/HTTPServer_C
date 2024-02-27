#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void launch(struct Server *server)
{
    char buffer[40000];
    char *hello = "<html><body><h1>Hola equipo de XXX</h1></body></html>\n";
    //char *hello = "НТТР/1.1 200 0K\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32) \nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Type: text/html\nConnection: Closed\n\n<html><body><h1>Hello, World!</h1></body></html>";
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