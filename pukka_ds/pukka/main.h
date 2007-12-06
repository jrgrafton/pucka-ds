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
const u16 SCREENHOLE =48;

const u16 BORDER = 4;	//Border in pixels around screen (used for collisions)

//Defines for max x and y for an entity to be
const u16 MAXX = 255 - BORDER;
const u16 MAXY = 191 - BORDER;
const u16 MINXY = 0+BORDER;

//Function definitions
void debug(s16 xpos,s16 ypos,char* message);
void applyFriction(entity* object);
void doCollisions(void);
void doDrawing(void);
void init(void);
void print_debug(void);
void processInput(void);


#endif

