#ifndef PR_SERVERCONNECTION_H
#define PR_SERVERCONNECTION_H


#include <WinSock2.h>
#include "ServerGame.h"
#include "Game.h"
#include "List.h"


#define PORT "1337"


typedef struct SrvThreads {
	HANDLE mainThread;
	HANDLE threadRecv;
	HANDLE threadSnd;
}SrvThreads_t;


typedef struct SrvConnInfo {
	ADDRINFOA* addrInfo;
	SOCKET socket;
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


#endif