#define _CRT_SECURE_NO_WARNINGS
#include <Winsock2.h>
#include <ws2tcpip.h>
#include "ClientConnection.h"
#include "ClientGame.h"


/*typedef struct {
	char* nick;
	ALLEGRO_EVENT event;
} Message;

typedef  struct {
	char* nick;
	ALLEGRO_EVENT_QUEUE* queue;
} Klient_dane;


void sendMessage(Message message) {};

DWORD WINAPI wysylanie(void* args) {
	Klient_dane* dane = (Klient_dane*)args;
	ALLEGRO_EVENT_QUEUE* queue = dane->queue;
	char* nick = dane->nick;

	bool running = true;
	while (running) {
		ALLEGRO_EVENT event;

		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.type == ALLEGRO_KEY_ESCAPE)
			running = false;

		if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN || event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
			Message message = (Message) { nick, event };
			sendMessage(message);
		}
	}
}*/


DWORD WINAPI _clnt_start_thread_recv(LPVOID params) {
	ClientConnection_t* conn = (ClientConnection_t*)params;
	unsigned partNumber, byteSum, bytesToBeReceived[2], bytesReceived = 0;
	char *data, isReceivingData = 0;

	while (recv(conn->socket, (char*)bytesToBeReceived, sizeof(unsigned) * 2, 0) > 0) {
		byteSum = bytesToBeReceived[0] + bytesToBeReceived[1];
		data = (char*)malloc(sizeof(byteSum));
		bytesReceived = 0;
		while (bytesReceived != byteSum) {
			bytesReceived += recv(conn->socket, data + bytesReceived, byteSum - bytesReceived, 0);
		}
	}

	Board_t* board = unpack_board(data, bytesToBeReceived[1], bytesToBeReceived[0]);
}


DWORD WINAPI _clnt_start_thread_snd(LPVOID params) {

}


void connect_to_server(ClientConnection_t* conn) {
	PADDRINFOA addrInfo = conn->addrInfo;
	while (addrInfo != NULL) {
		connect(conn->socket, addrInfo->ai_addr, addrInfo->ai_addrlen);
		addrInfo = addrInfo->ai_next;
	}
	conn->threads.threadSnd = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)_clnt_start_thread_snd, (LPVOID)conn, 0, NULL);
	_clnt_start_thread_recv((LPVOID)conn);
}


ClientConnection_t* create_client_connection(char* ip) {
	ADDRINFOA *addrInfo, hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(ip, CLNTPORT, &hints, &addrInfo) != 0) {
		freeaddrinfo(addrInfo);
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