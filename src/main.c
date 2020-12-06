#include <stdio.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_error.h>

#define NUM_DOTS    1024
#define LEN_PIX     800
#define FL_LEN_PIX  800.0f
#define WID_PIX     600
#define FL_WID_PIX  600.0f

/* Struct */
typedef struct
{
  uint8_t red, green, blue;
  float vx, vy; // velocity
  float x, y; //xypos
} dot;

/* Globals */
SDL_Surface* demo_screen;
// time
struct timespec ts;
float demo_time_measure = 0.0f;
float demo_time_step = 0.0f;
// surface
dot demo_dots[NUM_DOTS];

float demo_convert_time(struct timespec *ts)
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

float demo_roll()
{
  float r;
  r = (float)(rand()%RAND_MAX);;
  r /= (float)(RAND_MAX-1);
  return r;
}

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

void demo_draw()
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

int main(int argn, char **argv)
{
  if(SDL_Init((Uint32)SDL_INIT_VIDEO) != 0)
    fprintf(stderr,"SDL Init Failed!!! %s\n", SDL_GetError()); 
  else
    printf("SDL Init success!");

  demo_screen = SDL_SetVideoMode(LEN_PIX, WID_PIX, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if(!demo_screen)
    fprintf(stderr, "SetVideoMode failed: %s\n", SDL_GetError());
  
  /* evt loop */
  SDL_Event evt;
  int active = 1;
  uint8_t red = 64U;
  uint8_t green = 128U;
  uint8_t blue = 0U;
  uint8_t alpha = 0U;

  demo_init();

  while(active)
  {
    while(SDL_PollEvent(&evt))
    {
      if(evt.type == SDL_QUIT)
        active = 0;
    }
    demo_start_time();
    demo_handler();
    SDL_FillRect(demo_screen, NULL, SDL_MapRGBA(demo_screen->format, (red%128) + 127, (green%128) + 127, blue++, alpha));
    // SDL_FillRect(demo_screen, NULL, SDL_MapRGBA(demo_screen->format, 0, 0, 255, alpha));
    green += 2U;
    red += 8U;
    alpha += 24U;
    demo_draw();
    SDL_Flip(demo_screen);
    demo_end_time();
  }


  /* Shut down */
  SDL_Quit();
  printf("SDL Shut down!");
  printf("Hi!\n");
  return 0;
}
