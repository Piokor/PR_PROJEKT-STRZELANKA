#ifndef PR_CLIENTCONNECTION_H
#define PR_CLIENTCONNECTION_H


#include <WinSock2.h>
#include "ServerGame.h"
#include "GameShared.h"
#include "List.h"


#define CLNTPORT "1337"


typedef struct ClientThreads {
	HANDLE threadRecv;
	HANDLE threadSnd;
}ClntThreads_t;


typedef struct ClientConnection {
	SOCKET socket;
	ADDRINFOA* addrInfo;
	ClntThreads_t threads;
}ClientConnection_t;



DWORD WINAPI _clnt_start_thread_recv(LPVOID params);
DWORD WINAPI _clnt_start_thread_snd(LPVOID params);
void connect_to_server(ClientConnection_t* conn);
ClientConnection_t* create_client_connection(char* ip);


#endif