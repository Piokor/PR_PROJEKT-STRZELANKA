#ifndef PR_SERVERCONNECTION_H
#define PR_SERVERCONNECTION_H


#include <WinSock2.h>
#include "ServerGame.h"
#include "GameShared.h"
#include "List.h"


#define SRVPORT "1337"


typedef struct SrvThreads {
	HANDLE mainThread;
	HANDLE threadRecv;
	HANDLE threadSnd;
}SrvThreads_t;


typedef struct SrvConnInfo {
	ADDRINFOA* addrInfo;
	SOCKET socket;
	char playerNick[NICK_LEN];
}SrvConnInfo_t;


typedef struct SrvMutexes {
	HANDLE boardMutex;
	HANDLE sendMutex;
}SrvMutexes_t;


typedef struct ShooterServer {
	SrvConnInfo_t srvInfo;
	ListHandle_t* playerConnections;
	SrvThreads_t threads;
	SrvMutexes_t mutexes;
	SrvGameData_t* gameData;
}ShooterServer_t;


DWORD WINAPI _srv_start_thread_rcv(LPVOID params);
DWORD WINAPI _srv_start_thread_snd(LPVOID params);
void start_server(ShooterServer_t* srv);
ShooterServer_t* create_server();


#endif