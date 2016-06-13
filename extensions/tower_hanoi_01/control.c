#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "tower_struct.h"
#include "control.h"
//#include "show.h"

SDL_Event event;
extern int num_of_elem;
int currentTower = -1;
int dest_tower = -1;

int keys[2] = {-1, -1};

/* Check the keyboard state every tick of the loop. */
//const Uint8 *states = SDL_GetKeyState(NULL);


void controlKey_from(SDLKey key){
  switch(key){
    case SDLK_1:
      currentTower = 0;
      //pop(currentTower);
      break;
    case SDLK_2:
      currentTower = 1;
      //pop(currentTower);
      break;
    case SDLK_3:
      currentTower = 2;
      //pop(currentTower);
      break;
    case SDLK_r: reset(num_of_elem); break;
    case SDLK_ESCAPE: exit(0); break;

    default    : break;
  }
}

/*
void controlKey_to(SDLKey key){
  switch(key){
    case SDLK_1: dest_tower = 0; break;
    case SDLK_2: dest_tower = 1; break;
    case SDLK_3: dest_tower = 2; break;
    default    : break;
   }
  // move(currentTower, dest_tower);
}
*/
/*
void second_input(void) {
  printf("Please input the second tower: \n");
  switch(event.type){
    case SDL_QUIT: exit(0); break;
    case SDL_KEYDOWN:
      controlKey_to(event.key.keysym.sym);
      printf("the second input is %i\n", dest_tower);
      printf("current is :%i\n", currentTower);
      printf("dest is :%i\n", dest_tower);

      if (currentTower != -1 && dest_tower != -1) {
        move(currentTower, dest_tower);
      }
      break;
    default: break;
  }
}*/

void main_control(void){
  //printf("Please input the first tower: \n");
  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN:
        controlKey_from(event.key.keysym.sym);
        if (keys[0] == -1) {
          keys[0] = currentTower;
        } else if (keys[0] != -1 && keys[1] == -1) {
          keys[1] = currentTower;
          move(keys[0], keys[1]);
          keys[0] = -1;
          keys[1] = -1;
        }

        //printf("the first input is %i\n", keys[0]);

      //    second_input();
      // }
       //move(currentTower, dest_tower);
        break;
      //case SDL_KEYUP:
      default: break;
    }
  }
}



///////////////////////
