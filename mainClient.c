#include "ClientGame.h"
#include "ClientConnection.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include "GameShared.h"


int mainClient() {
	ALLEGRO_DISPLAY* display = NULL;

	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();


	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}

	ALLEGRO_FONT* font = al_load_ttf_font("arial.ttf", FONT_SIZE, 0);
	al_install_keyboard();
	al_install_mouse();

	const char* nick = insertNick();
	const char* ip = insertIP();

	display = al_create_display(BOARD_SIZE_X, BOARD_SIZE_Y);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	};

	al_clear_to_color(al_map_rgb(20, 80, 150));

	ClientConnection_t* connection = create_client_connection(ip);
	connect_to_server(connection);

	al_destroy_display(display);

	return 0;
}