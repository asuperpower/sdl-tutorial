#include <stdio.h>
#include <time.h>
#include <SDL/SDL.h>
#include <SDL/SDL_error.h>
#include "main.h"
#include "demo.h"

/* Globals */
SDL_Surface* demo_screen;
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
    demo_draw(demo_screen);
    SDL_Flip(demo_screen);
    demo_end_time();
  }


  /* Shut down */
  SDL_Quit();
  printf("SDL Shut down!");
  printf("Hi!\n");
  return 0;
}
