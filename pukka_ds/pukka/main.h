#ifndef __MAIN1__
#define __MAIN1__

//Type defs
typedef struct{
	u16 x, y; // point in centre of entity
	s16 vx, vy; // xy velocity
	s16 speed;
	s16 angle;
	s16 spriteIndex;
	u16 width, height; //Dimensions in pixels
	u16 radius;
}entity;

//Const definitions
#define DEBUG				//Comment out to turn off debug mode
const u16 SCREENHOLE =48;	//How big percieved hole between screens is
const u16 BORDER = 4;	//Border in pixels around screen (used for collisions)

//Defines for width and height of screen
const u16 SHEIGHT = 191;
const u16 SWIDTH = 255;

//Defines for max x and y for an entity to be
const u16 MAXX = SWIDTH - BORDER;
const u16 MAXY = SHEIGHT - BORDER;
const u16 MINX = 0+BORDER;
const u16 MINY = 0+BORDER;

//Function definitions
void debug(s16 xpos,s16 ypos,char* message);
void applyFriction(entity* object);
void doCollisions(void);
void doDrawing(void);
void init(void);
void print_debug(void);
void processInput(void);


#endif

