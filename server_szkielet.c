#include "server_szkielet.h"
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>


#define PORT 1337
#define MAX_PLAYERS 20
#pragma comment(lib, "Ws2_32.lib")


void server_critical_error() {
	printf("An error has occured! The server is shutting down...\n");
}


DWORD WINAPI _srv_start_player_thread_rcv(LPVOID params) {
	SrvConnInfo_t* playerConn = (SrvConnInfo_t*)params;

}


DWORD WINAPI _srv_start_player_thread_snd(LPVOID params) {
	SrvConnInfo_t* playerConn = (SrvConnInfo_t*)params;

}


DWORD WINAPI _srv_start_main_thread(LPVOID params) {
	ShooterServer_t* srv = (ShooterServer_t*)params;
	if (bind(srv->srvInfo.srvSocket, srv->srvInfo.addrInfo->ai_addr, (int)srv->srvInfo.addrInfo->ai_addrlen) == SOCKET_ERROR ||
		listen(srv->srvInfo.srvSocket, MAX_PLAYERS) == SOCKET_ERROR) {
		shutdown(srv->srvInfo.srvSocket, SD_BOTH);
		return FALSE;
	}

	for (;;) {
		SrvConnInfo_t* newPlayerConn = &((srv->playerConnections)[(srv->connectedPlayers)++]);
		newPlayerConn->srvSocket = accept(srv->srvInfo.srvSocket, 
			newPlayerConn->addrInfo->ai_addr, newPlayerConn->addrInfo->ai_addrlen);
		newPlayerConn->threads.threadRecv = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)_srv_start_player_thread_rcv, (LPVOID)newPlayerConn, 0, NULL);
		newPlayerConn->threads.threadSnd = CreateThread(NULL, 0,
			(LPTHREAD_START_ROUTINE)_srv_start_player_thread_snd, (LPVOID)newPlayerConn, 0, NULL);
	}
}


void start_server(ShooterServer_t* srv) {
	srv->srvInfo.thread = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)_srv_start_main_thread, (LPVOID)srv, 0, NULL);
}


ShooterServer_t* create_server() {
	ShooterServer_t* srv = (ShooterServer_t*)malloc(sizeof(ShooterServer_t));
	ADDRINFOA *addrInfo, hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "1337", &hints, &addrInfo) != 0) {
		free(srv);
		return NULL;
	}
	
	SOCKET mainRecSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	if (mainRecSocket == INVALID_SOCKET) {
		free(srv);
		return NULL;
	}

	srv->srvInfo.addrInfo = addrInfo;
	srv->srvInfo.srvSocket = mainRecSocket;
	srv->playerConnections = (SrvConnInfo_t*)malloc(sizeof(SrvConnInfo_t) * MAX_PLAYERS);
	srv->connectedPlayers = 0;
	return srv;
}