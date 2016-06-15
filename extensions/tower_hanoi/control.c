#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "tower_struct.h"
#include "control.h"

SDL_Event event;
extern int num_of_elem;
/* Initialise for input */
int currentTower = -1;

/* Save the pair of inputs */
int keys[2] = {-1, -1};

/* Get input key */
void controlKey_get(SDLKey key) {
  switch(key) {
    case SDLK_KP1:
    case SDLK_1: currentTower = 0; break;
    case SDLK_KP2:
    case SDLK_2: currentTower = 1; break;
    case SDLK_KP3:
    case SDLK_3: currentTower = 2; break;
    case SDLK_r: keys[0] = -2; break;
    case SDLK_ESCAPE: exit(0); break;
    default    : break;
  }
}

void main_control(void) {
  while(SDL_PollEvent(&event)) {
    switch(event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN:
        controlKey_get(event.key.keysym.sym);
        if (keys[0] == -2) {  // call resetting
          /* Reset the input */
          keys[0] = -1;
          keys[1] = -1;
          reset(num_of_elem);
        } else if (keys[0] == -1) {  // case of don't have the first key yet
          keys[0] = currentTower;
        } else if (keys[0] != -1 && keys[1] == -1) { // have 1st but not 2nd
          keys[1] = currentTower;
          /* Apply movement after getting a pair of inputs */
          move(keys[0], keys[1]);
          /* Reset the input */
          keys[0] = -1;
          keys[1] = -1;
        }
        break;
      default: break;
    }
  }
}
