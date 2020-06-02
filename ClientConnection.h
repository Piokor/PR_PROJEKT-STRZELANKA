#ifndef PR_CLIENTCONNECTION_H
#define PR_CLIENTCONNECTION_H


#include <WinSock2.h>
#include "GameShared.h"
#include "List.h"


#define CLNTPORT "1338"


typedef struct ClientConnection {
	SOCKET socket;
	ADDRINFOA* addrInfo;
	HANDLE threadRecv;
	char* nick;
}ClientConnection_t;


typedef union {
	char nick[NICK_LEN];
	ALLEGRO_EVENT event;
}Package_t;


DWORD WINAPI _clnt_start_thread_recv(LPVOID params);
DWORD WINAPI _clnt_start_thread_snd(LPVOID params);
void connect_to_server(ClientConnection_t* conn);
ClientConnection_t* create_client_connection(char* ip, char* nick);


#endif