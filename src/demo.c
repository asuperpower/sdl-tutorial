#include <stdio.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_video.h>
#include <SDL/SDL_error.h>
#include "demo.h"

// structs
/* Struct */
typedef struct
{
  uint8_t red, green, blue;
  float vx, vy; // velocity
  float x, y; //xypos
} dot;

// time
struct timespec ts;
float demo_time_measure = 0.0f;
float demo_time_step = 0.0f;
// surface
dot demo_dots[NUM_DOTS];

// static function definitions
static float demo_convert_time(struct timespec *ts);
static float demo_roll();

// api functions
void demo_init()
{
  for (int i=0; i < NUM_DOTS; i++)
  {
    demo_dots[i].red = (uint8_t)(rand()  % 255U);
    demo_dots[i].green = (uint8_t)(rand() % 255U);
    demo_dots[i].blue = (uint8_t)(rand() % 255U);
    demo_dots[i].vx = demo_roll()*16.0f-8.0f;
    demo_dots[i].vy = demo_roll()*16.0f-8.0f;
    demo_dots[i].x = demo_roll()*FL_LEN_PIX;
    demo_dots[i].y = demo_roll()*FL_WID_PIX;
  }
}

void demo_start_time()
{
  struct timespec ts;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  demo_time_measure = demo_convert_time(&ts);
}

void demo_end_time()
{
  struct timespec ts;
  float delta;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  delta = demo_convert_time(&ts) - demo_time_measure; // dt
  demo_time_step = delta/(1000.0f/32.0f); 
}

void demo_handler()
{
  for(int i=0; i < NUM_DOTS; i++)
  {
    demo_dots[i].x += demo_dots[i].vx*demo_time_step;
    demo_dots[i].y += demo_dots[i].vy*demo_time_step;
    // bounce
    if(demo_dots[i].x < 0.0f || demo_dots[i].x >= 320.0f)
    {
      demo_dots[i].x -= demo_dots[i].vx*demo_time_step;
      demo_dots[i].vx = -demo_dots[i].vx;
    }
    if(demo_dots[i].y < 0.0f || demo_dots[i].y >= 320.0f)
    {
      demo_dots[i].y -= demo_dots[i].vy*demo_time_step;
      demo_dots[i].vy = -demo_dots[i].vy;
    }
  }
}

void demo_draw(SDL_Surface* demo_screen)
{
  int i=0;
  // int rank=0;
  int x=0;
  int y=0;
  //int rank, x, y;
  Uint32 *pixel;
  //lock while we make changes
  SDL_LockSurface(demo_screen);
  // rank = demo_screen->pitch/sizeof(Uint32);
  pixel=(Uint32*)demo_screen->pixels;
  //draw
  for(i=0; i < NUM_DOTS; i++)
  {
    x = (int)demo_dots[i].x;
    y = (int)demo_dots[i].y;
    Uint32 colour = SDL_MapRGBA(demo_screen->format, demo_dots[i].red, demo_dots[i].green, 0U,255U );
    pixel[x+y * (WID_PIX-1)] = colour;//*rank] = colour;
  }
  SDL_UnlockSurface(demo_screen);
}

// static functions
static float demo_convert_time(struct timespec *ts)
{
  float accu;
  // combine into floating number
  accu = (float)ts->tv_sec; // seconds 
  accu *= 1000000000.0f; /* One second is 1000x1000x1000 nanoseconds, s -> ms, us, ns */
  accu += (float)ts->tv_nsec; //ns
  //ms range, ns res
  accu /= 1000000.0f;
  return accu;
}

static float demo_roll()
{
  float r;
  r = (float)(rand()%RAND_MAX);;
  r /= (float)(RAND_MAX-1);
  return r;
}


