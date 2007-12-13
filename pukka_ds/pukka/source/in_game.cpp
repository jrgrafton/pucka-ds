//Internal headers
#include "../header/main.h"

// PAGfxConverter Include
#include "../source/gfx/all_gfx.h"
#include "../source/gfx/all_gfx.c"

//Fields of game entitys
GameObject * puck;
GameObject * handle;

//Lookup table for square root used to calculate overlap distance
//For it to get to that point x and y locations of entitys must be within
//At least the product of their radii (give or take 10% for fast function innacruacies)
//Hence LUT should be (34*34) = 1156 big
u32 sqrtLUT[1156];

//Has a goal just been scored
int goal = 0;

//Is the stylus currently hooked to the handle
int hooked =0;

/**
**InGame constructor
**/
InGame::InGame() : State(){
	init();
	myName = INGAME;
}

/**
**InGame constructor
**/
InGame::~InGame(){

}
/**
**InGame init allocate resources
**/
void InGame::init(){
	//Init libs
	//this.myName = INGAME;

	PA_SetBgPalCol(0, 1, PA_RGB(10,10, 0));
	PA_Init8bitBg(0,0);

	PA_EasyBgLoad(1, // screen
			1, // background number (0-3)
			table_top); // Background name, used by PAGfx...
	PA_EasyBgLoad(0, // screen
			2, // background number (0-3)
			table_bottom); // Background name, used by PAGfx...
	
	

	//Init text on top screen background 0
	PA_InitText(1,0); // On the top screen
	PA_SetTextCol(1,0,0,0);
	
	//Dual sprite means treat both screens as one
	//This loads palate that can be used by boths screens (pallate number,ref to pallate data)
	//Note atm same palate is being used for both sprites
	PA_DualLoadSpritePal(0, (void*)puck_image_Pal);
	PA_DualLoadSpritePal(1, (void*)handle_image_Pal);
	
	// This'll be the handle->..(only visable on bottom screen)
	handle = new GameObject();
	PA_CreateSprite(0, 0,(void*)handle_image_Sprite, OBJ_SIZE_32X32,1, 1, 16, 16); 	
	handle->spriteIndex=0;
	handle->width=handle->height=32;
	handle->radius=16;
	
	// This will be the puck (sprite number,reference to sprite, size of sprite,colour mode,pallate,xloc,yloc)
	puck = new GameObject();
	PA_DualCreateSprite(1,(void*)puck_image_Sprite, OBJ_SIZE_32X32,1, 0, 128-16, 96-16); 
	puck->spriteIndex=1;
	puck->width=puck->height=30;
	puck->radius=15;

	//Reset state
	reset();
	
	//Set the virtual distance in screen to SCREENHOLE
	PA_SetScreenSpace(SCREENHOLE);

	//Initialise sqrtLUT
	for(u32 i =0;i<LUTSIZE;i++){
		sqrtLUT[i] = (u32)sqrt(i);
	}
}
/**
**InGame reset reset all positions speeds scores and trajectorys
**/
void InGame::reset(){
	puck->x = SWIDTH/2<<8; 
	puck->y = (96+192+SCREENHOLE)<<8; // central position on bottom screen
	puck->vx = 0; 
	puck->vy = 0; // No trajectory
	puck->speed = 0;

	handle->x = SWIDTH/2<<8; 
	handle->y = SHEIGHT-BORDER-handle->radius<<8;
	handle->vx = 0;
	handle->vy = 0;
	handle->speed = 0;
}
/**
**InGame run
**/
void InGame::run(){
	while(1)
	{
		//Process input
		processInput();
		
		//Do collisions
		doCollisions();

		//Do drawing
		doDrawing();
		
		//Print debug stuff (if in debug mode)
		#ifdef DEBUG
			print_debug();
		#endif

		//Wait for vsync
		PA_WaitForVBL();
	}
}
/**
Process input function
**/
void InGame::processInput(void){
	//Update stylus position
	PA_UpdateStylus();
	
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
	if (PA_SpriteTouched(handle->spriteIndex)||hooked){
		handle->x = sx<<8;
		handle->y = sy<<8;
		hooked=1;
	}

	//If we moved sprite out of bounds then move it back (limit is 255x 191y)
	s16 hx = handle->getX();
	s16 hy = handle->getY();
	
	hx = (hx-handle->radius<MINX)? handle->radius+MINX:(hx+handle->radius>MAXX)? MAXX-handle->radius:hx;
	hy = (hy-handle->radius<MINY)? handle->radius:(hy+handle->radius>MAXY)? MAXY-handle->radius:hy;
	
	handle->x=hx<<8;
	handle->y=hy<<8;
}

/**
Do drawing function
**/
void InGame::doDrawing(void){
	// Since collision is done between top right of one object and centre of moving puc must offset by half sprite size (16px)
	//This draws the moving sprite
	PA_DualSetSpriteXY(puck->spriteIndex, puck->getX()-16, puck->getY()-16);
	
	//Draw the handle
	PA_SetSpriteXY(0, // screen
		handle->spriteIndex, // sprite
		handle->getX()-16, // x position
		handle->getY()-16); // y...

	//Draw the goals
	//Bottom one
	PA_Draw8bitLineEx(0,(SWIDTH/2)-(GOALWIDTH/2),PHEIGHT-GOALHEIGHT,(SWIDTH/2)+(GOALWIDTH/2), PHEIGHT-GOALHEIGHT,1,1);  
}	

/**
Do collisions function
**/
void InGame::doCollisions(void){
	//Get the square of the max distance
	u16 maxDistance = (puck->radius+handle->radius)*(puck->radius+handle->radius);
	// Collision between puck and handle note PA-DISTANCE returns the distance squared 
	if (PA_Distance(handle->getX(), handle->getY(), puck->getX(), puck->getY()-192-SCREENHOLE) < maxDistance) {
		u32 overlapDistance = ((puck->radius+handle->radius)-getDistance(handle->getX(), handle->getY(), puck->getX(), puck->getY()-192-SCREENHOLE));
		
		
		handle->speed = ((u16)(fabs(Stylus.oldVx)+fabs(Stylus.oldVy))/2)+1;
		if(puck->speed>0){puck->speed-=2;}
		if(handle->speed>puck->speed){puck->speed=handle->speed;}
		if(puck->speed>20){puck->speed=20;}
		

		u16 angle = PA_GetAngle(handle->getX(), handle->getY(), puck->getX(), puck->getY()-192-SCREENHOLE); 

		puck->angle = angle;
		puck->vx = PA_Cos(angle);
		puck->vy = -PA_Sin(angle);
		
		//Displace handle so its outside of puck (move handle away from pucks trajectory)
		handle->x -= puck->vx*overlapDistance;
		handle->y -= puck->vy*overlapDistance;

		//Displace puck so its outside of handle move (move puck along its trajectory)
		puck->x += puck->vx*overlapDistance;
		puck->y += puck->vy*overlapDistance;
	}
	
	//Add velocity to current puck position (one that moves by itself)
	puck->x += puck->vx*puck->speed;
	puck->y += puck->vy*puck->speed;
	
	// Collision with left or right walls
	if ((puck->getX() - puck->radius < MINX)) {
		puck->vx = -puck->vx;
		puck->x=(MINX+puck->radius)<<8;
	}
	else if ((puck->getX() + puck->radius > MAXX)){
		puck->vx = - puck->vx;
		puck->x=(MAXX-puck->radius)<<8;
	}

	// Collision with top or bottom walls
	if ((puck->getY() -puck->radius < MINY)){
		puck->vy = -puck->vy;
		puck->y=(MINY+puck->radius)<<8;
	}

	else if ((puck->getY() + puck->radius > SHEIGHT + MAXY + SCREENHOLE)){
		//Have we let in a goal?
		if(puck->getX()>(SWIDTH/2)-(GOALWIDTH/2)&&puck->getX()<(SWIDTH/2)+(GOALWIDTH/2)){
			goal = 1;
			puck->vx = 0;
			//Has the puck gone all the way into the goal?
			if(puck->getY()-puck->radius>SHEIGHT + MAXY + SCREENHOLE){
				goalScored();
			}
		}
		else{
			puck->vy = - puck->vy;
			puck->y= (SHEIGHT + MAXY + SCREENHOLE - puck->radius)<<8;
		}
	}
}

/**
** Get Proper Euclidian dist
**/
u32 InGame::getDistance(s16 x1,s16 y1,s16 x2,s16 y2){
	u32 lookupValue = square(x1-x2)+square(y1-y2);
	//Add one to because of float -> int truncation
	u32 result = sqrtLUT[lookupValue]+1;
	return result;
}
/**
** Inline square function
**/
inline u32 InGame::square(u32 a){ return a*a;}

/**
Temporary debug function, put everything here that you
wanna print to screen
**/
void InGame::print_debug(void){
	//Debug stuff for velocity (printf in lib doesnt support \n)
	PA_ClearTextBg(1);
	PA_OutputText(1, 0, 1, "Velocity is: x:%d y:%d",puck->vx,puck->vy);
	PA_OutputText(1, 0, 2, "Positon is: x:%d y:%d",puck->getX(),puck->getY());
	PA_OutputText(1, 0, 3, "Speed is: %d",handle->speed);
	PA_OutputText(1, 0, 4, "Angle: %d",puck->angle);
	PA_OutputText(1, 0, 5, "Goal is: %d",goal);
}

/**
**Function that gets called when someone scores
**/
void InGame::goalScored(){
	reset();
	goal=0;
	hooked =0;
}
