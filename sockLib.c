/*
    Author: Mohamed Ahmed
    E-mail: 9eek.mohamed@gmail.com
    Twitter: @9eekM
    version: 1.0.0
    description:
        socket library make it easier to programming linux socket client/server programs
        with new way to dealing with client.
*/

#include <signal.h>     // contral the SIGPIPE
#include "sockLib.h"


int _init_SERVER(int * _connected_d, struct sockaddr_info* _info, int _port, int __n){

    int sock;
    int reuseAddr_true = 1;
    unsigned int sockaddr_size = sizeof(struct sockaddr);

    if ((sock=socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    _info->server.sin_family = AF_INET;
    _info->server.sin_port = (in_port_t)htons(_port);
    _info->server.sin_addr.s_addr = INADDR_ANY;
    bzero(&_info->server.sin_zero, 0);

    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseAddr_true, sizeof(int)) < 0)
        return -1;

    // ignore the signal which sent by clint if it terminated
    signal(SIGPIPE, SIG_IGN);

    if(bind(sock, (struct sockaddr*)&_info->server, sockaddr_size) < 0)
        return -1;

    if(listen(sock, __n))
        return -1;

    while(1)
    {
        *_connected_d = accept(sock, (struct sockaddr*)&_info->client, &sockaddr_size);
        if (*_connected_d > 0)
        {
            return 0;
        }
        else if (*_connected_d == 0)
        {
            return -1;
        }

    }
    return -1;
}

int s_communicate(int * _sock){

    int sent,received;
    char income_data[BUFF],out_data[BUFF];

    while(1)
    {
        // get the commend form user and replace \n with \0
        printf("HackToon~#> ");
        fgets(out_data, BUFF, stdin);
        out_data[strlen(out_data)] = '\0';

        if (strcmp(out_data, "exit\n") == 0){
            send(*_sock, "exit", 5, 0);
            close(*_sock);
            break;
        } // to close the connection by user (exit).

        sent = send(*_sock, out_data, strlen(out_data), 0);

        if (sent==-1){
            puts("[error] Client Terminated or Crashed.");
            close(*_sock);
            exit(1);
        } // if the clint terminated.

        received = recv(*_sock, income_data, BUFF, 0);
        income_data[received-1] = '\0';

        if (received>0)
        {
            if (strcmp(income_data,"0") == 0){
                puts("[error] Can't find this command !.");
            }else if(strcmp(income_data,"1")){
                printf("%s\n",income_data);
            }
        }
    }
    return 0;
}


int _init_CLI(int * _sock, struct sockaddr_in * _cli, char * _addr, int _port)
{
    int connected = -1;

    if((*_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return -1;

    _cli->sin_family = AF_INET;
    _cli->sin_port = (in_port_t)htons(_port);
    _cli->sin_addr.s_addr = inet_addr(_addr);
    bzero(&_cli->sin_zero, 0);

    while(connected == -1)
    {
        connected = connect(*_sock, (struct sockaddr*)_cli, sizeof(struct sockaddr));
    }
    return 0;
}

int c_communicate(int * _sock, void * _methods[])
{
    int bytes_recv, cmp_reuslt;
    char income[BUFF];

    while(1)
    {
        bytes_recv = recv(*_sock, income, BUFF, 0);
        income[bytes_recv-1] = '\0';

        if (strcmp(income,"exit") == 0)
        {
            close(*_sock);
            exit(0);
        }// catch exit command and terminate the clinet

        cmp_reuslt = scan_income(_sock, income, _methods);

        if (cmp_reuslt == -1)
        {
            send(*_sock, ERROR, 2, 0);
        }
        else if (cmp_reuslt == 0)
        {
            send(*_sock, SUCCESS, 2, 0);
        }
        else
        {
            send(*_sock, "[error] Unknowen error", 23, 0);
        }
    }
    return 0;
}

int scan_income(int * _sock ,const char * _cmd,void * _methods[])
{
    int key,func;
    for (key=0,func=1; func <= (int)sizeof_methods(_methods); key+=2,func+=2)
    {
        if(strcmp(_cmd, _methods[key]) == 0){
            int (*handler)(int) = _methods[func];
            if (handler(*_sock) != 0)
                return 1; // error in code execution
            return 0; // code execution success
        }
    }
    return -1; // can't find command in list
}

float sizeof_methods(void* _methods[])
{
    float num = 0;
    float *tester = &num;
    while (tester!=NULL)
    {
        tester = _methods[(int)num];
        num++;
    }
    return num-1; // return number of items without NULL value
}
