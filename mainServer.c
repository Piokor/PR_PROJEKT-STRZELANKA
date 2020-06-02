#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <allegro5/allegro.h>
#include "ServerConnection.h"



int main(int argc, char* argv[]) {
	ShooterServer_t* srv = create_server();
	start_server(srv);

	return 0;
}