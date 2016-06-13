#ifndef SHOW
#define SHOW


/* screen settings, might move these later ?? */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP    32


#define TOWER_STATE_NORMAL  0
#define TOWER_STATE_HOVER   1
#define TOWER_STATE_CLICKED 2


/* surfaces */
SDL_Surface *screen     = NULL;
SDL_Surface *background = NULL;
SDL_Surface *win        = NULL;
SDL_Surface *rod        = NULL;
SDL_Surface *ring1      = NULL;
SDL_Surface *ring2      = NULL;
SDL_Surface *ring3      = NULL;
SDL_Surface *ring4      = NULL;
SDL_Surface *ring5      = NULL;
SDL_Surface *ring6      = NULL;

/* tower clipping coordinates
SDL_Rect towerClip[3];
SDL_Rect ringClip[3];
SDL_Rect menuClip[12];
*/
/* tower positions */
const int towerPos[] = { 100, 305, 510 };
/*extern SDL_Surface *background;*/


int loadSurface( SDL_Surface **surface, char *filename );
void loadImages(void);
void cleanUp(void);
void apply_surface( int x, int y, SDL_Surface *source, SDL_Surface *destination);
void setBackground (SDL_Surface *BGimage);
void drawRing(int towNum, int ringNum, int ringTop);
void drawTower(int towNum, int ring[]);
void drawScreen(void);
void init(void);
void selectRing(void);




#endif
