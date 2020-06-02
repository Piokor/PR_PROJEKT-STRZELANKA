#define _CRT_SECURE_NO_WARNINGS
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include "ServerConnection.h"
#include "GameShared.h"
#include "List.h"
#include "PlayerConnectionList.h"
#include "ServerGame.h"
#include "ClientConnection.h"
#pragma comment(lib, "Ws2_32.lib")


DWORD WINAPI _srv_start_thread_rcv(LPVOID params) {
	ShooterServer_t* srv = (ShooterServer_t*)params;
	ListElem_t* playerConnElem;
	SrvConnInfo_t* playerConn;
	unsigned bytesReceived = 0;
	Package_t eventPackage;

	for (;;) {
		if (srv->playerConnections->isAllocated) {
			playerConnElem = srv->playerConnections->head;
			while(playerConnElem != NULL){
				playerConn = (SrvConnInfo_t*)(playerConnElem->data);
				while (bytesReceived != sizeof(Package_t)) {
					bytesReceived += recv(playerConn->socket,
						(char*)(&eventPackage) + bytesReceived, sizeof(Package_t) - bytesReceived, 0);
				}
				parse_event(&(eventPackage.event), srv->gameData->srvBoard, playerConn->playerNick);
				bytesReceived = 0;
				playerConnElem = playerConnElem->next;
			}
		}
	}
	return TRUE;
}


DWORD WINAPI _srv_start_thread_snd(LPVOID params) {
	ShooterServer_t* srv = (ShooterServer_t*)params;
	ListElem_t* currConnection = NULL;
	SrvConnInfo_t* connInfo = NULL;
	BoardPacked_t* dataToSend = NULL;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	ALLEGRO_EVENT event;
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	ALLEGRO_TIMER* timer = al_create_timer(0.001 * SRV_REFRESH_TIME_MS);
	al_register_event_source(queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	for (;;) {
		al_wait_for_event(queue, &event);
		WaitForSingleObject(srv->boardMutex, INFINITE);
		dataToSend = pack_board(srv->gameData->srvBoard);
		ReleaseMutex(srv->boardMutex);

		currConnection = srv->playerConnections->head;
		while (currConnection != NULL) {
			connInfo = (SrvConnInfo_t*)(currConnection->data);
			send(connInfo->socket, (char*)(dataToSend->bytes), sizeof(unsigned) * 2, 0);
			send(connInfo->socket, (char*)(dataToSend->data), (dataToSend->bytes)[0] + (dataToSend->bytes)[1], 0);
			currConnection = currConnection->next;
		}

		free(dataToSend->data);
	}

	return TRUE;
}


void start_server(ShooterServer_t* srv) {
	static DWORD nonBlock = 1;

	if (bind(srv->srvInfo.socket, srv->srvInfo.addrInfo->ai_addr, (int)srv->srvInfo.addrInfo->ai_addrlen) == SOCKET_ERROR ||
		listen(srv->srvInfo.socket, PLAYERS_LIMIT) == SOCKET_ERROR) {
		shutdown(srv->srvInfo.socket, SD_BOTH);
	}

	SrvConnInfo_t* newPlayerConn;
	Package_t nickPackage;
	unsigned bytesReceivedNick = 0;
	for (;;) {
		newPlayerConn = malloc(sizeof(SrvConnInfo_t));

		newPlayerConn->socket = accept(srv->srvInfo.socket,
			newPlayerConn->addrInfo->ai_addr, &(newPlayerConn->addrInfo->ai_addrlen));
		ioctlsocket(newPlayerConn->socket, FIONBIO, &nonBlock);

		while (bytesReceivedNick != sizeof(Package_t)) {
			bytesReceivedNick += recv(newPlayerConn->socket, 
				(char*)(&nickPackage) + bytesReceivedNick, sizeof(Package_t) - bytesReceivedNick, 0);
		}
		strcpy(newPlayerConn->playerNick, nickPackage.nick);

		WaitForSingleObject(srv->boardMutex, INFINITE);  // MUTEX (BINARNY SEMAFOR)
		insert_end(srv->playerConnections, newPlayerConn);
		ReleaseMutex(srv->boardMutex);
	}
}


ShooterServer_t* create_server() {
	ShooterServer_t* srv = (ShooterServer_t*)malloc(sizeof(ShooterServer_t));
	ADDRINFOA *addrInfo, hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, SRVPORT, &hints, &addrInfo) != 0) {
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
	srv->boardMutex = CreateMutex(NULL, FALSE, NULL);

	return srv;
}