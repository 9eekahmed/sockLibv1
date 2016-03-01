#include <stdio.h> /* for input and output BIFs */
#include <stdlib.h> /* exit() function */
#include <unistd.h> /* close() function */
#include <string.h> /* strcmp() function */
#include <sys/socket.h> /* socket, bind, listen, setsockopt, accept and connect functions */
#include <arpa/inet.h> /* sockaddr_in, sockaddr structure */
#include <netinet/in.h> /* inet_ntoa and inet_addr functions */

#define BUFF 1024
#define ERROR "0"
#define SUCCESS "1"

struct sockaddr_info{
    struct sockaddr_in server;
    struct sockaddr_in client;
};

// initialize new socket and wait for new connection form client then return 1 (true)
int _init_SERVER(int *connected_d, struct sockaddr_info* info, int _port, int __n);

// start communicating with client and send , recev data and porcess it
int s_communicate(int * _sock);

// initialize new socket and wait for new connection form server then return 0 (true)
int _init_CLI(int *_sock, struct sockaddr_in*_cli, char *_addr, int _port);

// start communicateing with server and process the commands which sent by server and replay.
int c_communicate(int *sock, void *_methods[]);

// scan if income command is in commands list and return 0 if it is, -1 if not and 1 if it can't process the code
int scan_income(int * _sock ,const char * _cmd,void * _methods[]);

// return number of item in the list with out NULL value .
float sizeof_methods(void* _methods[]);
