#define _CRT_SECURE_NO_WARNINGS
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "ServerConnection.h"
#include "GameShared.h"
#include "List.h"
#include "PlayerConnectionList.h"
#include "ServerGame.h"
#pragma comment(lib, "Ws2_32.lib")


void server_critical_error() {
	printf("An error has occured! The server is shutting down...\n");
}


DWORD WINAPI _srv_start_thread_rcv(LPVOID params) {
	SrvConnInfo_t* playerConn = (SrvConnInfo_t*)params;
	char buf[256];
	while (recv(playerConn->socket, buf, sizeof(buf), 0) > 0) {
		printf("\n%s\n", buf);
	}
	return TRUE;
}


DWORD WINAPI _srv_start_thread_snd(LPVOID params) {
	ShooterServer_t* srv = (ShooterServer_t*)params;
	ListElem_t* currConnection = NULL;
	SrvConnInfo_t* connInfo = NULL;
	BoardPacked_t* dataToSend = NULL;

	for (;;) {
		WaitForSingleObject(srv->mutexes.sendMutex, INFINITE);

		WaitForSingleObject(srv->mutexes.boardMutex, INFINITE);
		dataToSend = pack_board(srv->gameData->srvBoard);
		ReleaseMutex(srv->mutexes.boardMutex);

		currConnection = srv->playerConnections->head;
		while (currConnection != NULL) {
			connInfo = (SrvConnInfo_t*)(currConnection->data);
			send(connInfo->socket, (char*)dataToSend, dataToSend->bytes, 0);
			currConnection = currConnection->next;
		}
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
		SrvConnInfo_t* newPlayerConn = malloc(sizeof(SrvConnInfo_t));

		newPlayerConn->socket = accept(srv->srvInfo.socket, 
			newPlayerConn->addrInfo->ai_addr, &(newPlayerConn->addrInfo->ai_addrlen));

		WaitForSingleObject(srv->mutexes.boardMutex, INFINITE);  // MUTEX (BINARNY SEMAFOR)
		insert_end(srv->playerConnections, newPlayerConn);
		ReleaseMutex(srv->mutexes.boardMutex);
	}
}


void start_server(ShooterServer_t* srv) {
	srv->threads.mainThread = CreateThread(NULL, 0, 
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
	srv->playerConnections = init_list_handle_pcn();
	srv->gameData = init_srv_game_data();
	srv->mutexes.boardMutex = CreateMutex(NULL, FALSE, NULL);
	srv->mutexes.sendMutex = CreateMutex(NULL, TRUE, NULL);

	return srv;
}