#pragma once
// macros
#define NUM_DOTS    1024
#define LEN_PIX     800
#define FL_LEN_PIX  800.0f
#define WID_PIX     600
#define FL_WID_PIX  600.0f


#include <SDL/SDL_video.h> // for SDL_Surface*

// functions
void demo_init();
void demo_start_time();
void demo_end_time();
void demo_handler();
void demo_draw(SDL_Surface* demo_screen);
