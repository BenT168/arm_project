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
      if (num_of_elem == Max_elem) { // in case highest level
        win = SDL_LoadBMP("images/winning.bmp");
        setBackground(win);
        return;
      } else { // in case can go to next level
        next_level();
        return;
      }
    }
    /* Put background image */
    setBackground(background);

    /* Draw three towers */
    drawTower(0, towers[0]);
    drawTower(1, towers[1]);
    drawTower(2, towers[2]);
}

/* Read player's input for number of rings */
void getRingNum(SDLKey key) {
  switch (key) {
    case SDLK_ESCAPE: exit(0); break;
    case SDLK_KP3:
    case SDLK_3: num_of_elem = 3; break;
    case SDLK_KP4:
    case SDLK_4: num_of_elem = 4; break;
    case SDLK_KP5:
    case SDLK_5: num_of_elem = 5; break;
    case SDLK_KP6:
    case SDLK_6: num_of_elem = 6; break;
    default: break;
  }
}

/* put the ending image */
void endGame() {
  end = SDL_LoadBMP("images/ending.bmp");
  setBackground(end);
  SDL_Flip(screen);
}

/* ask if want to go to next level */
void next_level() {
  nextLevel = SDL_LoadBMP("images/next_level.bmp");
  setBackground(nextLevel);
  SDL_Flip(screen);

  next: ;
  SDL_Event next_event;

  while (SDL_PollEvent(&next_event)) {
    setBackground(nextLevel);
    SDL_Flip(screen);
    switch(next_event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYUP:
        switch(next_event.key.keysym.sym){
          case SDLK_n: break;
          default :
          break;
        }
      case SDL_KEYDOWN:
        switch (next_event.key.keysym.sym) {
          case SDLK_ESCAPE: exit(0); break;
          case SDLK_r: // restart this level
            reset(num_of_elem);
            return;
            break;
          case SDLK_RETURN:
          case SDLK_KP_ENTER:
          case SDLK_y:       // go to next level
            num_of_elem ++;
            initialise(num_of_elem);
            /* game loop */
            while(gameRunning == 1) {
              main_control();  // get SDL key
              drawScreen();    //update screen
              SDL_Flip(screen);
              SDL_Delay(16);
            }
            cleanUp();
            return;
            break;
          case SDLK_n:  // not continue
            endGame();
            break;
          default: goto next; break;
        }
        break;
      default: goto next; break;
    }
  }
  goto next;

}

/* The main Function */
int main(int argc, char **argv) {

  SDL_Init(SDL_INIT_VIDEO);

  /* Set the screen */
  screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SCREEN_BPP, SDL_SWSURFACE);

  /* Set the instruction page */
  instruction = SDL_LoadBMP("images/welcome.bmp");
  setBackground(instruction);
  SDL_Flip(screen);

  /* Checking input for Initialise the number of rings */
  main: ;
  SDL_Event check_event;

  while (SDL_PollEvent(&check_event)) {
    switch(check_event.type) {
      case SDL_QUIT: exit(0); break;
      case SDL_KEYDOWN:
        getRingNum(check_event.key.keysym.sym);  // Read input
        if (3 <= num_of_elem && num_of_elem <= 6) {
          goto init;
        } else {  // if the input is not a number 3-6
          goto main;
        }
        break;
      default: break;
    }
  }
  goto main;

  init: ;
  /* Initialise the game */
  initialise(num_of_elem);

  background = SDL_LoadBMP("images/background.bmp");
  if(background == NULL){
    exit(1);
  }

  /* game loop */
  while(gameRunning == 1) {
    main_control();  // get SDL key
    drawScreen();    //update screen
    SDL_Flip(screen);
    SDL_Delay(16);
  }

  cleanUp();

  return 0;
}
