#ifndef PR_SERVERCONNECTION_H
#define PR_SERVERCONNECTION_H


#include <WinSock2.h>
#include "ServerGame.h"
#include "Game.h"


#define PORT "1337"


typedef struct SrvThreads {
	HANDLE threadRecv;
	HANDLE threadSnd;
}SrvThreads_t;


typedef struct SrvConnInfo {
	ADDRINFOA* addrInfo;
	SOCKET socket;
	union {
		SrvThreads_t threads;
		HANDLE thread;
	};
}SrvConnInfo_t;


typedef struct ShooterServer {
	SrvConnInfo_t srvInfo;
	unsigned connectedPlayers;
	SrvConnInfo_t* playerConnections;
	SrvGameData_t* gameData;
}ShooterServer_t;


#endif