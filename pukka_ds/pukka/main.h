#ifndef __MAIN1__
#define __MAIN1__

//Type defs
typedef struct{
	s16 x, y; // point in centre of entity
	s16 vx, vy; // xy velocity
	s16 speed;
	s16 angle;
	s16 spriteIndex;
}entity;

//Const definitions
#define DEBUG				//Comment out to turn off debug mode
const int SCREENHOLE =48;

//Function definitions
void debug(s16 xpos,s16 ypos,char* message);
void applyFriction(entity* object);
void doCollisions(void);
void doDrawing(void);
void init(void);
void print_debug(void);
void processInput(void);


#endif

