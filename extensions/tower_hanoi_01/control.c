#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "tower_struct.h"
#include "control.h"
//#include "show.h"

SDL_Event event;
extern int num_of_elem;
int currentTower;
int dest_tower;



void main_control_in(void){
  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym){
          case SDLK_ESCAPE : exit(0); break;
          case SDLK_r      : reset(num_of_elem); break;
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
          default: break;
        }
      case SDL_KEYUP:
        case SDLK_1: break;
        case SDLK_2: break;
        case SDLK_3: break;
        default    : break;
    }
  }
}




void main_control_out(void){
  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_KEYDOWN:
        switch(event.key.keysym.sym){
          case SDLK_ESCAPE : exit(0); break;
          case SDLK_r      : reset(num_of_elem); break;
          case SDLK_1:
            dest_tower = 0;
            push(dest_tower, pop(currentTower));
            move(currentTower, dest_tower);
            printf("destination tower: %i\n",dest_tower );
            break;
          case SDLK_2:
            dest_tower = 1;
            push(dest_tower, pop(currentTower));
            move(currentTower, dest_tower);
            break;
          case SDLK_3:
            dest_tower = 2;
            push(dest_tower, pop(currentTower));
            move(currentTower, dest_tower);
            break;
          default: break;
        }
      case SDL_KEYUP:
        case SDLK_1: break;
        case SDLK_2: break;
        case SDLK_3: break;
        default    : break;
    }
  }
}
