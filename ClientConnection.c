#define _CRT_SECURE_NO_WARNINGS
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "ClientConnection.h"
#include "ClientGame.h"


DWORD WINAPI _clnt_start_thread_recv(LPVOID params) {
	ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", FONT_SIZE, 0);
	ClientConnection_t* conn = (ClientConnection_t*)params;
	unsigned byteSum, bytesToBeReceived[2], bytesReceived = 0;
	char *data, isReceivingData = 0;

	while (recv(conn->socket, (char*)bytesToBeReceived, sizeof(unsigned) * 2, 0) > 0) {
		byteSum = bytesToBeReceived[0] + bytesToBeReceived[1];
		data = (char*)malloc(sizeof(byteSum));

		bytesReceived = 0;
		while (bytesReceived != byteSum) {
			bytesReceived += recv(conn->socket, data + bytesReceived, byteSum - bytesReceived, 0);
		}

		Board_t* board = unpack_board(data, bytesToBeReceived[1], bytesToBeReceived[0]);
		draw_board(board, font);
		free_board(board);
		free(data);
	}

	return TRUE;
}


DWORD WINAPI _clnt_start_thread_snd(LPVOID params) {
	ClientConnection_t* conn = (ClientConnection_t*)params;
	char running = 1;
	Package_t package;
	ALLEGRO_EVENT event = { 0 };
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());


	while (running) {
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.type == ALLEGRO_KEY_ESCAPE) {
			running = false;
		}
		else if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN ||
			event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
			package.event = event;
			send(conn->socket, (char*)(&package), sizeof(Package_t), 0);
		}
	}

	return TRUE;
}


void connect_to_server(ClientConnection_t* conn) {
	PADDRINFOA addrInfo = conn->addrInfo;
	while (addrInfo != NULL) {
		connect(conn->socket, addrInfo->ai_addr, addrInfo->ai_addrlen);
		addrInfo = addrInfo->ai_next;
	}

	// Wys³anie nicku
	Package_t package;
	strcpy(package.nick, conn->nick);
	send(conn->socket, (char*)(&package), sizeof(Package_t), 0);

	conn->threadRecv = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_clnt_start_thread_recv, (LPVOID)conn, 0, NULL);
	_clnt_start_thread_snd((LPVOID)conn);

	TerminateThread(conn->threadRecv, 0);
}


ClientConnection_t* create_client_connection(char* ip, char* nick) {
	ADDRINFOA *addrInfo, hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(ip, CLNTPORT, &hints, &addrInfo) != 0) {
		return NULL;
	}

	ClientConnection_t* clientConnection = (ClientConnection_t*)malloc(sizeof(ClientConnection_t));
	clientConnection->socket = socket(addrInfo->ai_family, addrInfo->ai_socktype, addrInfo->ai_protocol);
	if (clientConnection->socket == INVALID_SOCKET) {
		free(clientConnection);
		freeaddrinfo(addrInfo);
		return NULL;
	}

	clientConnection->addrInfo = addrInfo;
	return clientConnection;
}