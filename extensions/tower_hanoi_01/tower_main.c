#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "tower_struct.h"
#include "show.h"

//#include <pigpio.h>


//SDL_Surface *screen = NULL;
//SDL_Surface *background = NULL;
SDL_Surface *sprite = NULL;

int gameRunning = 1;
SDL_Event event;

/*
void apply_surface(int x,int y, SDL_Surface* source, SDL_Surface* destination){
  SDL_Rect rectangle;
  rectangle.x = x;
  rectangle.y = y;

  SDL_BlitSurface(source, NULL, destination, &rectangle);
}
*/
int main(int argc, char **argv){
  int num_of_elem;

  main: ;
  printf("Please enter the number of elements you want: ");

  //printf("There are %d elements to start with.\n", scanf("%d\n", &x));

 scanf(" %d", &num_of_elem);
 //printf("you type in %i\n", num_of_elem);
  if (num_of_elem == 0) {
    printf("Your input is not avilible.\n");
    return -1;
    //goto main;
  } else if (num_of_elem < 3 || num_of_elem > Max_elem) {
    printf("The number you typed in is not avilible.\
     Please type numbers in the range 3-%i\n", Max_elem);
    goto main;
  }
  printf("the input is : %i\n", num_of_elem);


  initialise(num_of_elem);
  //Init
  SDL_Init(SDL_INIT_VIDEO);

  //load
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  background = SDL_LoadBMP("images/background_math.bmp");
  if(background == NULL){
    exit(1);
  }
  sprite = SDL_LoadBMP("images/tower-of-hanoi.bmp");
  if(sprite == NULL){
    exit(1);
}


//Game Loop
while(gameRunning == 1){

while(SDL_PollEvent(&event)){
  if((event.type) == SDL_QUIT){
    gameRunning = 0;
  }
}
/*
  //Apply surfaces
  apply_surface(0,0,background, screen);
  apply_surface(150, 150, sprite, screen);
*/

  drawScreen();
  //update screen
  SDL_Flip(screen);

  SDL_Delay(16);

}

/*
if(gpioInitialise() < 0){
  fprintf(stderr, "pigpio failed to initialise\n");
  return 1;
}
*/
/*Set GPIO modes */
//gpioSetMode(4, PI_OUTPUT);





atexit(SDL_Quit);
return 0;


}
