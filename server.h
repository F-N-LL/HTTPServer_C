#ifndef Server_h
#define Server_h

#include <sys/socket.h>
#include <netinet/in.h>


//Declaring main data structure convenient for our server porpuses
struct Server
{
   int domain;
   int service;
   int protocol;
   int port;
   u_long interface;
   int backlog;

   struct sockaddr_in address; //We are using IPv4, please be careful IPv6 vs IPv4
   
   int socket;

   void (*launch)(struct Server *server);
};

//Declaring constructor function structure with all params will be needed. Order of params is key.
struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void(*launch)(struct Server *server)); //function launch points to server memory address




#endif 