#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "tower_struct.h"
#include "show.h"
#include "control.h"

/* The number of rings set by player: 3-6 */
int num_of_elem;

/* Offsets of the background */
int backgroundX = 0;
int backgroundY = 0;

/* constant for running game */
int gameRunning = 1;

/* input event */
SDL_Event event;

void cleanUp(void) {
  /* free all surface */
  SDL_FreeSurface( background );
  SDL_FreeSurface( win );
  SDL_FreeSurface( rod );
  SDL_FreeSurface( ring1 );
  SDL_FreeSurface( ring2 );
  SDL_FreeSurface( ring3 );
  SDL_FreeSurface( ring4 );
  SDL_FreeSurface( ring5 );
  SDL_FreeSurface( ring6 );

  /* quit SDL */
  SDL_Quit();
}

/* Put image */
void apply_surface ( int x, int y, SDL_Surface *source,
                                   SDL_Surface *destination) {
  SDL_Rect rectangle;
  rectangle.x = x;
  rectangle.y = y;

  SDL_BlitSurface(source, NULL, destination, &rectangle );
}

/* Set the image as backgrand */
void setBackground (SDL_Surface *BGimage) {
  apply_surface (backgroundX, backgroundY, BGimage, screen);
}

/* Get proper ring image corresponding to the ring value */
SDL_Surface *getRingImg(int towerNum, int ringNum) {
  int ringValue = towers[towerNum][ringNum];
  SDL_Surface *ring = NULL;
  switch (ringValue) {
    case 6: ring = ring1; break; //RED
    case 5: ring = ring2; break; //ORANGE
    case 4: ring = ring3; break; //YELOOW
    case 3: ring = ring4; break; //GREEN
    case 2: ring = ring5; break; //BLUE
    case 1: ring = ring6; break; //VIOLET
    default:
      break;
  }
  return ring;
}

/* Put image for rings */
void drawRing(int towerNum, int ringNum, int ringHeight) {
  int x = 0;  // x coordinate for ring
  int width;  // width of rings in pixels

  /* Calculate width of the rings */
  int ringVal = towers[towerNum][ringNum];
  width = ringVal * 20 + 60;

  /* Get x coordinate for rings accoring to length */
  x = towerPos[towerNum] + 5 - width/2;

  /* Load images due to the number of rings */
  switch(num_of_elem) {
    case 6: ring1 = SDL_LoadBMP("images/red.bmp");
    case 5: ring2 = SDL_LoadBMP("images/orange.bmp");
    case 4: ring3 = SDL_LoadBMP("images/yellow.bmp");
    case 3:
      ring4 = SDL_LoadBMP("images/green.bmp");
      ring5 = SDL_LoadBMP("images/blue.bmp");
      ring6 = SDL_LoadBMP("images/violet.bmp");
    default:
      break;
  }
  /* Get image */
  SDL_Surface *ring = getRingImg(towerNum, ringNum);
  apply_surface(x, ringHeight, ring, screen);
}

/* Put image for towers */
void drawTower(int towerNum, int rings[]) {
  /* Draw the rods */
  rod      = SDL_LoadBMP("images/rod.bmp");
  hori_rod = SDL_LoadBMP("images/horizontal_rod.bmp");

  apply_surface(towerPos[towerNum], 120, rod, screen);
  apply_surface(towerPos[towerNum] - 95 , 420, hori_rod, screen);

  /* Draw ring by check values */
  for (int i = 0; i < num_of_elem; i ++) {
     drawRing(towerNum, i, 390 - (30*i));
  }
}

/* Drawr the whole screen */
void drawScreen(void) {
    /* If player wins put the winning image */
    if (check_win()) {
      win = SDL_LoadBMP("images/winning.bmp");
      setBackground(win);
      return;
    }

    /* Put background image */
    setBackground(background);

    /* Draw three towers */
    drawTower(0, towers[0]);
    drawTower(1, towers[1]);
    drawTower(2, towers[2]);
}


void init(void) {
    fprintf( stderr, "Initializing the screen...\n" );
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                           SDL_SWSURFACE );
    if (!screen)
    {
        fprintf(stderr, "Could not set video mode %dx%dx%d: %s",
                SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
                SDL_GetError());
        exit( EXIT_FAILURE);
    }


    /* set up clean shut down routine */
    atexit(cleanUp);

    fprintf(stderr, "Init completed successfully!\n");
}

/* The main Function */
int main(int argc, char **argv) {

  main: ;
  printf("Please enter the number of elements you want: ");

  /* Read an integer input */
  scanf(" %d", &num_of_elem);

  if (num_of_elem == 0) {
    printf("Your input is not avilible.\n");
    return -1;
  } else if (num_of_elem < 3 || num_of_elem > Max_elem) {
    printf("The number you typed in is not avilible.\
    Please type numbers in the range 3-%i\n", Max_elem);
    goto main;
  }

  /* Initialise the game */
  initialise(num_of_elem);

  SDL_Init(SDL_INIT_VIDEO);

  /* Set the screen */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SCREEN_BPP, SDL_SWSURFACE);
  background = SDL_LoadBMP("images/background.bmp");
  if(background == NULL){
    exit(1);
  }

  /* gmae loop */
  while(gameRunning == 1) {
    main_control();  // get SDL key
    drawScreen();    //update screen
    SDL_Flip(screen);
    SDL_Delay(16);
  }
  return 0;
}
