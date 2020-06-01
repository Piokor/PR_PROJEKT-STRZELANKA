#define _CRT_SECURE_NO_WARNINGS
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "ServerConnection.h"
#include "Game.h"
#pragma comment(lib, "Ws2_32.lib")


void server_critical_error() {
	printf("An error has occured! The server is shutting down...\n");
}


DWORD WINAPI _srv_start_player_thread_rcv(LPVOID params) {
	SrvConnInfo_t* playerConn = (SrvConnInfo_t*)params;
	char buf[256];
	while (recv(playerConn->socket, buf, sizeof(buf), 0) > 0) {
		printf("\n%s\n", buf);
	}
	return TRUE;
}


DWORD WINAPI _srv_start_player_thread_snd(LPVOID params) {
	SrvConnInfo_t* playerConn = (SrvConnInfo_t*)params;
	char msg[] = "Siema eniu!";
	int read = 0;
	for (;;) {
		Sleep(3500);
		send(playerConn->socket, msg, sizeof(msg), 0);
	}
	return TRUE;
}


DWORD WINAPI _srv_start_main_thread(LPVOID params) {
	ShooterServer_t* srv = (ShooterServer_t*)params;
	if (bind(srv->srvInfo.socket, srv->srvInfo.addrInfo->ai_addr, (int)srv->srvInfo.addrInfo->ai_addrlen) == SOCKET_ERROR ||
		listen(srv->srvInfo.socket, PLAYERS_LIMIT) == SOCKET_ERROR) {
		shutdown(srv->srvInfo.socket, SD_BOTH);
		return FALSE;
	}

	for (;;) {
		SrvConnInfo_t* newPlayerConn = &((srv->playerConnections)[(srv->connectedPlayers)++]);
		newPlayerConn->socket = accept(srv->srvInfo.socket, 
			newPlayerConn->addrInfo->ai_addr, &(newPlayerConn->addrInfo->ai_addrlen));
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

	if (getaddrinfo(NULL, PORT, &hints, &addrInfo) != 0) {
		free(srv);
		return NULL;
	}
	
	SOCKET mainRecSocket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	if (mainRecSocket == INVALID_SOCKET) {
		free(srv);
		return NULL;
	}

	srv->srvInfo.addrInfo = addrInfo;
	srv->srvInfo.socket = mainRecSocket;
	srv->playerConnections = (SrvConnInfo_t*)malloc(sizeof(SrvConnInfo_t) * PLAYERS_LIMIT);
	srv->connectedPlayers = 0;
	return srv;
}