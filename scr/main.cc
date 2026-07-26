#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/sprite.h>
#include <esat/time.h>

#include <esat_extra/soloud/soloud.h>
#include <esat_extra/soloud/soloud_wav.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ui_client.cc"
#include "sqlite_client.cc"
#include "../includes/sqlite_client.h"
#include "../includes/ui_client.h"
unsigned char fps = 60;


double current_time, last_time;

int esat::main(int argc, char **argv) {

	srand(time(NULL));

	esat::WindowInit(1080, 1920);
	esat::WindowSetMouseVisibility(true);


	while (esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {

		last_time = esat::Time();
		//Draw
		esat::DrawBegin();
		esat::DrawClear(0, 0, 0);

		switch (g_aplication_state)
		{
		case kSelectDataBase:
			DrawSelectionDB();
			break;
		case kClientPage:
			DrawClientPage();
			break;
		default:
			break;
		}

		esat::DrawEnd();
		
		
		do {
			current_time = esat::Time();
		} while ((current_time - last_time) <= 1000.0 / fps);

		esat::WindowFrame();
	}

	esat::WindowDestroy();

	return 0;
}