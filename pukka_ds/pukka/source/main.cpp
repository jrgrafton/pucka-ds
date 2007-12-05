// Includes
#include <PA9.h>

// PAGfxConverter Include
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"

//Custom header includes
#include "../main.h" 

//Fields of game entitys
entity puck;
entity handle;

int main(void){
	//Initialise everything
	init();
	
	while(1)
	{
		//Process input
		processInput();
		
		//Do collisions
		doCollisions();
		
		//Add velocity to current puck position (one that moves by itself)
		puck.x += puck.vx;
		puck.y += puck.vy;

		//Do drawing
		doDrawing();
	
		//Apply friction
		applyFriction(&puck);
		
		//Print debug stuff (if in debug mode)
		#ifdef DEBUG
			print_debug();
		#endif

		//Wait for vsync
		PA_WaitForVBL();
	}
	return 0;
}

/**
Process input function
**/
void processInput(void){
	//Update stylus position
	PA_UpdateStylus();
	
	//Are we hooked?
	static int hooked =0;
	
	//Get stylus position
	s16 sx = Stylus.X;
	s16 sy = Stylus.Y;
	
	//If stylus is released puck is no longer hooked
	if(!Stylus.Held){
		hooked=0;
	}

	//First check if we are hooked or touching
	if (PA_SpriteTouched(handle.spriteIndex)||hooked){
		handle.x = sx;
		handle.y = sy;
		hooked=1;
	}
	//If we moved sprite out of bounds then move it back (limit is 255x 191y)
	s16 hx = handle.x;
	s16 hy = handle.y;
	
	hx = (hx-16<0)? 16:(hx+16>255)? 255-16:hx;
	hy = (hy-16<0)? 16:(hy+16>191)? 191-16:hy;
	
	handle.x=hx;
	handle.y=hy;
}

/**
Do drawing function
**/
void doDrawing(void){
	// Since collision is done between top right of one object and centre of moving puc must offset by half sprite size (16px)
	//This draws the moving sprite
	PA_DualSetSpriteXY(1, puck.x-16, puck.y-16);
	PA_SetSpriteXY(0, // screen
		handle.spriteIndex, // sprite
		handle.x-16, // x position
		handle.y-16); // y...
}

/**
Do collisions function
**/
void doCollisions(void){
	// Collision between puck and handle
	if (PA_Distance(handle.x, handle.y, puck.x, puck.y-192-SCREENHOLE) < 32*32) {
		// Collision, so we'l change the pucks speed to move it out of our 'raquette'
		u16 angle = PA_GetAngle(handle.x, handle.y, puck.x, puck.y-192-SCREENHOLE); // New direction angle
		u16 speed = (32*32-PA_Distance(handle.x, handle.y, puck.x, puck.y-192-SCREENHOLE))/32; // The closer they are, the harder the hit was...
		speed+=1;
		puck.speed = speed;
		puck.angle = angle;
		puck.vx = (PA_Cos(angle)*puck.speed)>>8;
		puck.vy = -(PA_Sin(angle)*puck.speed)>>8;
	}

	// Collision with left or right walls
	if ((puck.x -16 <= 0) && (puck.vx < 0)) puck.vx = -puck.vx; 
	else if ((puck.x + 16 >= 256)&&(puck.vx > 0)) puck.vx = - puck.vx;

	// Collision with top or bottom walls
	if ((puck.y -16 <= 0) && (puck.vy < 0)) puck.vy = -puck.vy;
	else if ((puck.y + 16 >= 192 + 192 + SCREENHOLE)&& (puck.vy > 0)) puck.vy = - puck.vy;	
}

/**
Initialise everything
**/
void init(void){
	//Init libs
	PA_Init();
	PA_InitVBL();
	
	//Init text on top screen background 0
	PA_InitText(1,0); // On the top screen
	
	//Dual sprite means treat both screens as one
	//This loads palate that can be used by boths screens (pallate number,ref to pallate data)
	//Note atm same palate is being used for both sprites
	PA_DualLoadSpritePal(0, (void*)sprite0_Pal);
	
	// This'll be the movable sprite...(only visable on bottom screen)
	PA_CreateSprite(0, 0,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, 16, 16); 	
	handle.x = 16; 
	handle.y = 16;//(MAY NEED TO INITIALISE VELOCITY FIELDS AT LATER DATE)
	handle.spriteIndex=0;
	
	// This will be the hit circle (sprite number,reference to sprite, size of sprite,colour mode,pallate,xloc,yloc)
	PA_DualCreateSprite(1,(void*)circle_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16); 
	puck.x = 128; 
	puck.y = 96+192+SCREENHOLE; // central position on bottom screen
	puck.vx = 0; 
	puck.vy = 0; // No speed
	puck.spriteIndex=1;
	
	//Set the virtual distance in screen to SCREENHOLE
	PA_SetScreenSpace(SCREENHOLE);
}

/**
Temporary debug function, put everything here that you
wanna print to screen
**/
void print_debug(void){
	//Debug stuff for velocity (printf in lib doesnt support \n)
	PA_ClearTextBg(1);
	char velocitytext[80];
	char speedtext[80];
	char angletext[80];
	sprintf(velocitytext, "Velocity is: x:%d y:%d",puck.vx,puck.vy);
	sprintf(speedtext, "Speed is: %d",puck.speed);
	sprintf(angletext, "Angle is: %d",puck.angle);
	debug(0,1,velocitytext);
	debug(0,2,speedtext);
	debug(0,3,angletext);
}

/**
Print debug stuff to top screen
**/
void debug(s16 xpos,s16 ypos,char* message){
	PA_OutputText(1, xpos, ypos, message);
}

/**
Function to apply friction
**/
void applyFriction(entity* object){
	//object->speed-=1;
}
