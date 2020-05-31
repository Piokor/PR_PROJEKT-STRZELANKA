#ifndef PR_SERVER_H
#define PR_SERVER_H


#include <WinSock2.h>


typedef struct SrvThreads {
	HANDLE threadRecv;
	HANDLE threadSnd;
}SrvThreads_t;


typedef struct SrvConnInfo {
	ADDRINFOA* addrInfo;
	SOCKET srvSocket;
	union {
		SrvThreads_t threads;
		HANDLE thread;
	};
}SrvConnInfo_t;

typedef struct ShooterServer {
	SrvConnInfo_t srvInfo;
	unsigned connectedPlayers;
	SrvConnInfo_t* playerConnections;
}ShooterServer_t;


#endif