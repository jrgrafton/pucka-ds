//Internal headers
#include "../header/main.h"

//Sound headers
#include "puckhandlecollision.h"

// PAGfxConverter Include
#include "../source/gfx/all_gfx.h"
#include "../source/gfx/all_gfx.c"

//Fields of game entitys
GameObject * puck;
GameObject * handle;
GameObject * computerHandle;

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
	myName = INGAME;
	
	//Pallete for line drawing
	PA_SetBgPalCol(0, 1, PA_RGB(255,0, 0));
	PA_SetBgPalCol(1, 1, PA_RGB(255,0, 0));

	PA_Init8bitBg(0,0);
	PA_Init8bitBg(1,0);

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

	//Local pallete for top and bottom goals
	PA_DualLoadSpritePal(2,(void*)goal_Pal);
	
	// This'll be the handle->..(only visable on bottom screen)
	handle = new GameObject(); 	
	handle->spriteIndex=0;
	handle->width=handle->height=32;
	handle->radius=16;
	PA_DualCreateSprite(0,(void*)handle_image_Sprite, OBJ_SIZE_32X32,1, 1, SWIDTH/2-handle->radius,SHEIGHT*2+SCREENHOLE-BORDER-handle->radius);
	
	// This will be the puck (sprite number,reference to sprite, size of sprite,colour mode,pallate,xloc,yloc)
	puck = new GameObject();
	puck->spriteIndex=1;
	puck->width=puck->height=30;
	puck->radius=15;
	PA_DualCreateSprite(1,(void*)puck_image_Sprite, OBJ_SIZE_32X32,1, 0, SWIDTH/2, SHEIGHT+SCREENHOLE+SHEIGHT/2); 

	// This'll be the computer (only visable on top screen)
	computerHandle = new GameObject(); 	
	computerHandle->spriteIndex=2;
	computerHandle->width=handle->height=32;
	computerHandle->radius=16;
	PA_DualCreateSprite(2,(void*)handle_image_Sprite, OBJ_SIZE_32X32,1, 1, SWIDTH/2-computerHandle->radius,BORDER+handle->radius);
	
	//Load goals (bottom then top)
	PA_CreateSprite(0,3,(void*)goal_bottom_left_Sprite, OBJ_SIZE_64X64,1, 2, SWIDTH/2-64,SHEIGHT-64);
	PA_CreateSprite(0,4,(void*)goal_bottom_right_Sprite, OBJ_SIZE_64X64,1, 2, SWIDTH/2,SHEIGHT-64);
	PA_CreateSprite(1,3,(void*)goal_top_left_Sprite, OBJ_SIZE_64X64,1, 2, SWIDTH/2-64,0);
	PA_CreateSprite(1,4,(void*)goal_top_right_Sprite, OBJ_SIZE_64X64,1, 2, SWIDTH/2,0);
	
	//Make sure everything gets drawn in the right order
	PA_InitSpriteExtPrio(1); // Enable extended priorities
	PA_SetSpriteExtPrio(0, 0, 127);
	PA_SetSpriteExtPrio(0, 1, 127);
	PA_SetSpriteExtPrio(0, 2, 127);

	PA_SetSpriteExtPrio(1, 0, 127);
	PA_SetSpriteExtPrio(1, 1, 127);
	PA_SetSpriteExtPrio(1, 2, 127);

	PA_SetSpriteExtPrio(0, 3, 0);
	PA_SetSpriteExtPrio(0, 4, 0);
	PA_SetSpriteExtPrio(1, 3, 0);
	PA_SetSpriteExtPrio(1, 4, 0);


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
	puck->y = ((u32)((SHEIGHT*1.5)+SCREENHOLE))<<8; // central position on bottom screen
	puck->vx = 0; 
	puck->vy = 0; // No trajectory
	puck->speed = 0;

	handle->x = SWIDTH/2<<8; 
	handle->y = (SHEIGHT*2+SCREENHOLE-BORDER-handle->radius)<<8;
	handle->vx = 0;
	handle->vy = 0;
	handle->speed = 0;

	computerHandle->x = SWIDTH/2<<8; 
	computerHandle->y = BORDER+handle->radius<<8;
	computerHandle->vx = 0;
	computerHandle->vy = 0;
	computerHandle->speed = 0;
}
/**
**InGame run
**/
void InGame::run(){
	while(1)
	{
		//Process input
		processInput();
		
		//Do intel
		doIntel();

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
	s16 sy = Stylus.Y+SHEIGHT+SCREENHOLE;
	
	//If stylus is released puck is no longer hooked and set stylus velcoty to 0
	if(!Stylus.Held){
		hooked=0;
		Stylus.oldVy=0;
		Stylus.oldVx=0;
		handle->speed=0;
	}
	//Set speed of handle
	handle->speed = ((u16)(fabs(Stylus.oldVx)+fabs(Stylus.oldVy))/2)+1;

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
	hy = (hy-handle->radius<MINYS0)? handle->radius+MINYS0:(hy+handle->radius>MAXYS0)? MAXYS0-handle->radius:hy;
	
	handle->x=hx<<8;
	handle->y=hy<<8;
}
/**
Do intel function
**/
void InGame::doIntel(){
	//Only reac if puc is on our screen
	if(puck->getY()<SHEIGHT){
		if(computerHandle->x>puck->x){computerHandle->x-=(1<<8);}
		else{computerHandle->x+=(1<<8);}
		computerHandle->speed=2;
	}
}
/**
Do drawing function
**/
void InGame::doDrawing(void){
	// Since collision is done between top right of one object and centre of moving puc must offset by half sprite size (16px)
	
	//This draws the puck
	PA_DualSetSpriteXY(puck->spriteIndex, puck->getX()-16, puck->getY()-16);
	
	//Draw the handle
	PA_DualSetSpriteXY(handle->spriteIndex,handle->getX()-16,handle->getY()-16);
	
	//Draw computer
	PA_DualSetSpriteXY(computerHandle->spriteIndex,computerHandle->getX()-16,computerHandle->getY()-16);

	//Catch area of bottom one
	PA_Draw8bitLineEx(0,(SWIDTH/2)-(GOALWIDTH/2),SHEIGHT-BORDER-GOALHEIGHT-1,(SWIDTH/2)+(GOALWIDTH/2), SHEIGHT-BORDER-GOALHEIGHT-1,1,1); 
	//Catch area of top one
	PA_Draw8bitLineEx(1,(SWIDTH/2)-(GOALWIDTH/2),GOALHEIGHT+BORDER+1,(SWIDTH/2)+(GOALWIDTH/2),GOALHEIGHT+BORDER+1,1,1); 
}	

/**
Do collisions function
**/
void InGame::doCollisions(void){

	//Collision between handles and puck
	handlePuckCollision(puck,handle);
	handlePuckCollision(puck,computerHandle);
	
	//Collision between boundaries and game objects
	boundaryCheckPuck();
	boundaryCheck(computerHandle,MINX,MAXX,MINYS1,MAXYS1);
	
	//Add velocity to current puck position (one that moves by itself)
	puck->x += puck->vx*puck->speed;
	puck->y += puck->vy*puck->speed;
	
	
}

/**
** General boundary check routine
**/
void InGame::boundaryCheck(GameObject * gameObject,s32 minx,s32 maxx,s32 miny,s32 maxy){
	// Collision with left or right walls
	if ((gameObject->getX() - gameObject->radius < minx)) {
		gameObject->vx = -gameObject->vx;
		gameObject->x=(minx+gameObject->radius)<<8;
	}
	else if ((gameObject->getX() + gameObject->radius > maxx)){
		gameObject->vx = - gameObject->vx;
		gameObject->x=(maxx-gameObject->radius)<<8;
	}

	// Collision with top or bottom walls
	if ((gameObject->getY() -gameObject->radius < miny)){
		gameObject->vy = -gameObject->vy;
		gameObject->y=(miny+gameObject->radius)<<8;
	}

	else if ((gameObject->getY() + gameObject->radius > maxy)){
		gameObject->vy = - gameObject->vy;
		gameObject->y= (maxy - gameObject->radius)<<8;
	}
}

/**
** Special boundary check routine for puck because of goal scoring
**/
void InGame::boundaryCheckPuck(){
	// Collision with left or right walls
	if ((puck->getX() - puck->radius < MINX)) {
		puck->vx = -puck->vx;
		puck->x=(MINX+puck->radius)<<8; //Make sure that puck gets pushed back into game area
	}
	else if ((puck->getX() + puck->radius > MAXX)){
		puck->vx = - puck->vx;
		puck->x=(MAXX-puck->radius)<<8;
	}

	//Collision with top or bottom walls
	if ((puck->getY() -puck->radius < MINYDUAL)){
		//Have we scored a goal?
		if(puck->getX()>(SWIDTH/2)-(GOALWIDTH/2)&&puck->getX()<(SWIDTH/2)+(GOALWIDTH/2)){
			goal = 1;
			puck->vx = -1;
			//Has the puck gone all the way into the goal?
			if(puck->getY()+puck->radius<MINYDUAL){
				goalScored();
			}
		}
		else{
			puck->vy = -puck->vy;
			puck->y=(MINYDUAL+puck->radius)<<8;
		}
	}

	else if ((puck->getY() + puck->radius > MAXYDUAL)){
		//Have we let in a goal?
		if(puck->getX()>(SWIDTH/2)-(GOALWIDTH/2)&&puck->getX()<(SWIDTH/2)+(GOALWIDTH/2)){
			goal = 1;
			puck->vx = 0;
			//Has the puck gone all the way into the goal?
			if(puck->getY()-puck->radius>MAXYDUAL){
				goalScored();
			}
		}
		else{
			puck->vy = - puck->vy;
			puck->y= (MAXYDUAL - puck->radius)<<8;
		}
	}
}

/**
** Puck handle collision routine
**/
void InGame::handlePuckCollision(GameObject * puck,GameObject * handle){
	//Get the square of the max distance
	u16 maxDistance = (puck->radius+handle->radius)*(puck->radius+handle->radius);
	// Collision between puck and handle note PA-DISTANCE returns the distance squared 
	if (PA_Distance(handle->getX(), handle->getY(), puck->getX(), puck->getY()) < maxDistance) {

		//Play sound
		PA_PlaySimpleSound(PA_GetFreeSoundChannel(), puckhandlecollision);

		u32 overlapDistance = ((puck->radius+handle->radius)-getDistance(handle->getX(), handle->getY(), puck->getX(), puck->getY()));
		
		if(puck->speed>0){puck->speed-=2;}
		if(handle->speed>puck->speed){puck->speed=handle->speed;}
		if(puck->speed>20){puck->speed=20;}
		

		u16 angle = PA_GetAngle(handle->getX(), handle->getY(), puck->getX(), puck->getY()); 

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
