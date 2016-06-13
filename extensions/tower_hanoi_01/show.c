#include "SDL/SDL.h"
#include <stdlib.h>
#include <stdio.h>
#include "tower_struct.h"
#include "show.h"
#include "control.h"

//#define currentRing towers[0][0]
int num_of_elem;


/* display.c - graphic functions for hanoi */
//#define DISPLAY
//#include "constants.h"

//Offsets of the background
int backgroundX = 0;
int backgroundY = 0;


int gameRunning = 1;
SDL_Event event;
//SDL_Surface *sprite = NULL;


/*
SDL_Surface *loadImage( char *filename )
{
    SDL_Surface *tmpImg = NULL;
    SDL_Surface *optImg = NULL;

    // load the image to a temporary surface
    tmpImg = IMG_Load( filename );

    if (tmpImg)
    {
        // optimize it by changing it to display format
        optImg = SDL_DisplayFormat( tmpImg );
        // free the temporary surface
        SDL_FreeSurface( tmpImg );

        if (optImg)
        {
            // once we have an optimized image, we'll create and apply a
            // * colorkey
            Uint32 colorkey = SDL_MapRGB( optImg->format, 0, 0xFF, 0xFF);
            // apply the colorkey
            SDL_SetColorKey( optImg, SDL_SRCCOLORKEY, colorkey );
        }
    }
    return optImg;
}
*/
/* loadSurf: takes a (pointer to) a (pointer to) a surface, and a file name,
 *           then loads the file and points the pointer to the newly made
 *           image. Returns TRUE on success, FALSE on failure */
int loadSurface( SDL_Surface **surface, char *filename )
{
    /* output message */
    fprintf(stderr, "Loading %s...", filename);
    *surface = SDL_LoadBMP(filename);
    if (!surface)
    {
        fprintf(stderr,"\tERROR LOADING IMAGE: %s\n", filename);
        return 0;
    }
    fprintf(stderr, "\tImage loaded!\n");
    return 1;
}

/*
void loadImages(void)
{
    fprintf( stderr, "Loading all images...\n");
    if (!loadSurface(&background, "images/background.bmp")){
      exit(EXIT_FAILURE);
    }
    if (!loadSurface(&rod, "images/rod.bmp")) {
      exit(EXIT_FAILURE);
    }
    if (!loadSurface(&ring, "images/ring.bmp")){
      exit(EXIT_FAILURE);
    }
    if (!loadSurface(&win, "images/winning.bmp")){
      exit(EXIT_FAILURE);
    }

    fprintf( stderr, "All images loaded successfully!\n" );
}
*/
void cleanUp(void)
{
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

void apply_surface ( int x, int y, SDL_Surface *source, SDL_Surface *destination)
{
  SDL_Rect rectangle;
  rectangle.x = x;
  rectangle.y = y;

  SDL_BlitSurface(source, NULL, destination, &rectangle );
}

/* setBackground: applies provided surface to screen a the origin (0,0)
 *                with no clipping */
void setBackground (SDL_Surface *BGimage)
{
  apply_surface (backgroundX, backgroundY, BGimage, screen);
}

SDL_Surface *getRingImg(int tower, int ringNum) {
  int ringValue = towers[tower][ringNum];
  SDL_Surface *ring = NULL;
  switch (ringValue) {
    case 6: ring = ring1; break;
    case 5: ring = ring2; break;
    case 4: ring = ring3; break;
    case 3: ring = ring4; break;
    case 2: ring = ring5; break;
    case 1: ring = ring6; break;
    default:
      break;
  }
  return ring;
}

/* drawRing(): draws (ring) on (tower) with (ringTop) height offset */
void drawRing(int towerNum, int ringNum, int ringTop)
{
  int x = 0;              /* x coordinate for ring */
  int width_diff;       /* difference in ring size */
  SDL_Rect ringOff;    /* new offset, this changes the size of the ring */

  width_diff = (num_of_elem - ringNum) * 20;
  /* copy original offset */
  ringOff.x = 0;
  ringOff.y = 60;
  ringOff.w = 200 - width_diff;
  ringOff.h = 30;

  x = towerPos[towerNum];
  //if (ringNum == currentRing){
  //  x = towerPos[currentTower] - (ringOff.w/2) + 15;
//  } else {
    x = towerPos[towerNum] - 130 + (ringOff.w/2) + 15;
  //}

  switch(num_of_elem){
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
  /* Get corresponding ring image */
  //printf("getting ring %i in tower %i \n", ringNum, towerNum);
  //printf("the value we are getting is : %i\n", towers[towerNum][ringNum]);
  //printf("the y coordinate of this ring is : %i\n", ringTop);
  SDL_Surface *ring = getRingImg(towerNum, ringNum);
  apply_surface(x, ringTop, ring, screen);
}

/* drawTower: draws tower sprite and appropriate number of rings,
 *            according to the values in rings array. */
void drawTower(int towerNum, int rings[])
{
  /* draw the tower first */
  rod = SDL_LoadBMP("images/rod.bmp");
  apply_surface(towerPos[towerNum], SCREEN_HEIGHT-320, rod, screen);

  /* then draw the appropriate number of rings */
  for (int i = 0; i < num_of_elem; i ++) {
    //if (rings[i] != currentRing) {
     //push(currentTower, rings[i])
     //printf("printing ring %i in tower %i \n", i, towerNum);
     //printf("the value we are printing is : %i\n", rings[i]);
     //printf("the y coordinate of this ring is : %i\n", SCREEN_HEIGHT - 50 - (30*i));
     drawRing(towerNum, i, SCREEN_HEIGHT - 50 - (30*i));


  // } else {
     //push(currentTower, rings[i]);
  //   drawRing(tower, rings[i], 430);
  // }
  }
}


/* drawScreen(): draws current game state to screen */
void drawScreen(void)
{
    if (check_win())
    {
      setBackground(win);
      return;
    }

    /* setBackground sets the background image using apply_surface() */
    setBackground(background);

    drawTower(0, towers[0]);
    drawTower(1, towers[1]);
    drawTower(2, towers[2]);
}



void init(void)
{

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


    //SDL_WM_SetCaption( APP_TITLE, NULL );

    /* set up clean shut down routine */
    atexit(cleanUp);
    /* load images */
    //loadImages();

    fprintf(stderr, "Init completed successfully!\n");

    //currentRing = 0;
    //currentTower = 0;
}

/* selectRing(): selects the current ring. */
/*void selectRing(void)
{
    currentRing = peek(currentTower);
    if (currentRing)
        fprintf(stderr, "selectRing() ring selected: %d.\n", currentRing);
}*/



int main(int argc, char **argv){

  main: ;
  printf("Please enter the number of elements you want: ");

  scanf(" %d", &num_of_elem);

  if (num_of_elem == 0) {
    printf("Your input is not avilible.\n");
    return -1;
    //goto main;
  } else if (num_of_elem < 3 || num_of_elem > Max_elem) {
    printf("The number you typed in is not avilible.\
     Please type numbers in the range 3-%i\n", Max_elem);
    goto main;
  }
  //printf("the input is : %i\n", num_of_elem);


  initialise(num_of_elem);
  //Init
  SDL_Init(SDL_INIT_VIDEO);

  //load
  screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  background = SDL_LoadBMP("images/background_math.bmp");
  if(background == NULL){
    exit(1);
  }

  //Game Loop
  while(gameRunning == 1){

    main_control_in();
    main_control_out();
    
    drawScreen();
    //update screen
    SDL_Flip(screen);

    SDL_Delay(16);

 }


 //atexit(SDL_QUIT);
 return 0;
}
