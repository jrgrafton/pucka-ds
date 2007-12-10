// Includes
#include <PA9.h>
#include <math.h>

// PAGfxConverter Include
#include "gfx/all_gfx.h"
#include "gfx/all_gfx.c"

//Custom header includes
#include "../main.h" 

//Fields of game entitys
entity puck;
entity handle;

s16 dist = 0;

int main(void){
	//Initialise everything
	init();
	
	while(1)
	{
		//Process input
		processInput();
		
		//Do collisions
		doCollisions();

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
	
	//If stylus is released puck is no longer hooked and set stylus velcoty to 0
	if(!Stylus.Held){
		hooked=0;
		Stylus.oldVy=0;
		Stylus.oldVx=0;
	}


	//First check if we are hooked or touching
	if (PA_SpriteTouched(handle.spriteIndex)||hooked){
		handle.x = sx<<8;
		handle.y = sy<<8;
		hooked=1;
	}

	//If we moved sprite out of bounds then move it back (limit is 255x 191y)
	s16 hx = handle.getX();
	s16 hy = handle.getY();
	
	hx = (hx-handle.radius<MINX)? handle.radius+MINX:(hx+handle.radius>MAXX)? MAXX-handle.radius:hx;
	hy = (hy-handle.radius<MINY)? handle.radius:(hy+handle.radius>MAXY)? MAXY-handle.radius:hy;
	
	handle.x=hx<<8;
	handle.y=hy<<8;
}

/**
Do drawing function
**/
void doDrawing(void){
	// Since collision is done between top right of one object and centre of moving puc must offset by half sprite size (16px)
	//This draws the moving sprite
	PA_DualSetSpriteXY(puck.spriteIndex, puck.getX()-16, puck.getY()-16);
	
	//Draw the handle
	PA_SetSpriteXY(0, // screen
		handle.spriteIndex, // sprite
		handle.getX()-16, // x position
		handle.getY()-16); // y...
}

/**
Do collisions function
**/
void doCollisions(void){
	//Get the square of the max distance
	u16 maxDistance = (puck.radius+handle.radius)*(puck.radius+handle.radius);
	// Collision between puck and handle note PA-DISTANCE returns the distance squared 
	if (PA_Distance(handle.getX(), handle.getY(), puck.getX(), puck.getY()-192-SCREENHOLE) < maxDistance) {
		u16 overlapDistance = (maxDistance-PA_Distance(handle.getX(), handle.getY(), puck.getX(), puck.getY()-192-SCREENHOLE))/(puck.radius+handle.radius);
		
		u16 handleSpeed = ((u16)(fabs(Stylus.oldVx)+fabs(Stylus.oldVy))/2)+1;
		if(puck.speed>0){puck.speed-=2;}
		if(handleSpeed>puck.speed){puck.speed=handleSpeed;}
		

		u16 angle = PA_GetAngle(handle.getX(), handle.getY(), puck.getX(), puck.getY()-192-SCREENHOLE); 

		puck.angle = angle;
		puck.vx = PA_Cos(angle);
		puck.vy = -PA_Sin(angle);
		
		//Displace handle so its outside of puck (move handle away from pucks trajectory)
		handle.x -= puck.vx*overlapDistance;
		handle.y -= puck.vy*overlapDistance;

		//Displace puck so its outside of handle move (move puck along its trajectory)
		puck.x += puck.vx*overlapDistance;
		puck.y += puck.vy*overlapDistance;

		dist=overlapDistance;

	}
	
	//Add velocity to current puck position (one that moves by itself)
	//Note: divide by 257 instead of >>8 because of 2s compliment issues
	//E.g 510>>8 = 1 but -510>>8 = -2 (also not 256 cause that gets coverted
	//to bit shift automatically WOULD LIKE SOMEONE TO FULLY EXPLAIN THIS TO ME (JAMES GRAFTON)
	puck.x += puck.vx*puck.speed;
	puck.y += puck.vy*puck.speed;


	// Collision with left or right walls
	if ((puck.getX() - puck.radius < MINX)) {
		puck.vx = -puck.vx;
		puck.x=(MINX+puck.radius)<<8;
	}
	else if ((puck.getX() + puck.radius > MAXX)){
		puck.vx = - puck.vx;
		puck.x=(MAXX-puck.radius)<<8;
	}

	// Collision with top or bottom walls
	if ((puck.getY() -puck.radius < MINY)){
		puck.vy = -puck.vy;
		puck.y=(MINY+puck.radius)<<8;
	}

	else if ((puck.getY() + puck.radius > SHEIGHT + MAXY + SCREENHOLE)){
		puck.vy = - puck.vy;
		puck.y= (SHEIGHT + MAXY + SCREENHOLE - puck.radius)<<8;
	}
}

/**
Initialise everything
**/
void init(void){
	//Init libs
	PA_Init();
	PA_InitVBL();

	PA_EasyBgLoad(1, // screen
			1, // background number (0-3)
			table_top); // Background name, used by PAGfx...
	PA_EasyBgLoad(0, // screen
			1, // background number (0-3)
			table_bottom); // Background name, used by PAGfx...

	//Init text on top screen background 0
	PA_InitText(1,0); // On the top screen
	PA_SetTextCol(1,0,0,0);
	
	//Dual sprite means treat both screens as one
	//This loads palate that can be used by boths screens (pallate number,ref to pallate data)
	//Note atm same palate is being used for both sprites
	PA_DualLoadSpritePal(0, (void*)puck_image_Pal);
	PA_DualLoadSpritePal(1, (void*)handle_image_Pal);
	
	// This'll be the handle...(only visable on bottom screen)
	PA_CreateSprite(0, 0,(void*)handle_image_Sprite, OBJ_SIZE_32X32,1, 1, 16, 16); 	
	handle.x = 16<<8; 
	handle.y = 16<<8;//(MAY NEED TO INITIALISE VELOCITY FIELDS AT LATER DATE)
	handle.spriteIndex=0;
	handle.width=handle.height=32;
	handle.radius=16;
	
	// This will be the puck (sprite number,reference to sprite, size of sprite,colour mode,pallate,xloc,yloc)
	PA_DualCreateSprite(1,(void*)puck_image_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16); 
	puck.x = 128<<8; 
	puck.y = (96+192+SCREENHOLE)<<8; // central position on bottom screen
	puck.vx = 0; 
	puck.vy = 0; // No speed
	puck.spriteIndex=1;
	puck.width=puck.height=30;
	puck.radius=15;
	
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
	PA_OutputText(1, 0, 1, "Velocity is: x:%d y:%d",puck.vx,puck.vy);
	PA_OutputText(1, 0, 2, "Positon is: x:%d y:%d",puck.getX(),puck.getY());
	PA_OutputText(1, 0, 3, "Speed is: %d",puck.speed);
	PA_OutputText(1, 0, 4, "Angle: %d",puck.angle);
	PA_OutputText(1, 0, 5, "Overlap is: %d",dist);
}

/**
Function to apply friction
**/
void applyFriction(entity* object){
	//object->speed-=1;
}
