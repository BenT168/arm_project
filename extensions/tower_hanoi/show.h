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
SDL_Surface *screen     = NULL; // 640 * 480
SDL_Surface *background = NULL; // 640 * 480
SDL_Surface *win        = NULL; // 640 * 480
SDL_Surface *rod        = NULL; // 10  * 300
SDL_Surface *hori_rod   = NULL; // 200 * 10
SDL_Surface *ring1      = NULL; // 180 * 30, Red
SDL_Surface *ring2      = NULL; // 160 * 30, Orange
SDL_Surface *ring3      = NULL; // 140 * 30, Yellow
SDL_Surface *ring4      = NULL; // 120 * 30, Green
SDL_Surface *ring5      = NULL; // 100 * 30, Blue
SDL_Surface *ring6      = NULL; // 80  * 30, Voilet

const int towerPos[] = { 152, 314, 476 };

void cleanUp(void);
void apply_surface(int, int, SDL_Surface *, SDL_Surface *);
void setBackground (SDL_Surface *);
void drawRing(int, int, int);
void drawTower(int, int[]);
void drawScreen(void);
void init(void);





#endif
