//Internal headers
#include "../header/main.h"
#include "../header/game_object.h"
#include "../header/state.h"
#include "../header/in_game.h"
#include "../header/menu.h"
#include "../header/splash.h"

//Sound headers
#include "puckhandlecollision.h"
#include "player_score.h"
#include "computer_score.h"

//Fields of game entitys
GameObject * puck;
GameObject * handle;
GameObject * computerHandle;

//Lookup table for square root used to calculate overlap distance
//For it to get to that point x and y locations of entitys must be within
//At least the product of their radii (give or take 10% for fast function innacruacies)
//Hence LUT should be (68*68) = 4624 big
u32 sqrtLUT[LUTSIZE];

//Has a goal just been scored
int goal = 0;

//Is the stylus currently hooked to the handle
int hooked =0;

//Score
int playerScore=0;
int computerScore=0;

//Are we in game?
bool inGame;

//Array of numbers for score
void* allNumbers[10];

/**
**InGame constructor
**/
InGame::InGame(int level) : State(){
	inGame=1;
	init(level);
	myName = INGAME;
}

/**
**InGame constructor
**/
InGame::~InGame(){
	delete puck;
	delete handle;
	delete computerHandle;
	PA_ResetBgSys();
	PA_ResetSpriteSys();
	PA_InitSpriteExtPrio(0);
	PA_ClearTextBg(0);
	PA_StopSound(0);
}
/**
**InGame init allocate resources
**/
void InGame::init(int level){
	myName = INGAME;
	
	//Set the virtual distance in screen to SCREENHOLE
	PA_SetScreenSpace(SCREENHOLE);

	//Initialise sqrtLUT
	for(u32 i =0;i<LUTSIZE;i++){
		sqrtLUT[i] = (u32)sqrt(i);
	}
	computerScore=0;
	playerScore=0;
	
	currentLevel=level;
	obj_size=2;

	switch(currentLevel){
		case 0:
			computerImage = (void*)char1_image_Sprite;
			computerWidth = 32;
			intelPointer = &InGame::doIntel1;
		break;
		case 1:
			computerImage=(void*)char2_image_Sprite;
			computerWidth = 32;
			intelPointer = &InGame::doIntel2;
		break;
		case 2:
			computerImage=(void*)char3_image_Sprite;
			computerWidth = 32;
			intelPointer = &InGame::doIntel3;
		break;
		case 3:
			computerImage=(void*)char4_image_Sprite;
			computerWidth = 32;
			intelPointer = &InGame::doIntel4;
		break;
		case 4:
			computerImage=(void*)char5_image_Sprite;
			computerWidth = 64;
			intelPointer = &InGame::doIntel5;
			obj_size=3;
		break;
	}


	//Specify all stuff to do with level
	initGraphics();
}

void InGame::initGraphics(){
	#ifdef DEBUG
		PA_InitText(1,0); // On the top screen
		PA_SetTextCol(1,0,0,0);
	#endif

	PA_SetScreenSpace(SCREENHOLE);

	PA_EasyBgLoad(1, // screen
			1, // background number (0-3)
			table_top); // Background name, used by PAGfx...
	PA_EasyBgLoad(0, // screen
			1, // background number (0-3)
			table_bottom); // Background name, used by PAGfx...

	//Dual sprite means treat both screens as one
	PA_DualLoadSpritePal(0,(void*)char1_image_Pal);
	PA_DualLoadSpritePal(1,(void*)char2_image_Pal);
	PA_DualLoadSpritePal(2,(void*)char3_image_Pal);
	PA_DualLoadSpritePal(3,(void*)char4_image_Pal);
	PA_DualLoadSpritePal(4,(void*)char5_image_Pal);
	PA_DualLoadSpritePal(5,(void*)puck_image_Pal);
	PA_DualLoadSpritePal(6,(void*)handle_image_Pal);
	PA_DualLoadSpritePal(7,(void*)goal_Pal);
	PA_DualLoadSpritePal(8,(void*)goal_score_Pal);
	
	// This'll be the handle->..(only visable on bottom screen)
	handle = new GameObject(); 	
	handle->spriteIndex=0;
	handle->width=handle->height=32;
	handle->radius=16;
	handle->speed=0;
	PA_DualCreateSprite(0,(void*)handle_image_Sprite, OBJ_SIZE_32X32,1, 6, SWIDTH/2-handle->radius,SHEIGHT*2+SCREENHOLE-BORDER-handle->width);
	
	// This will be the puck (sprite number,reference to sprite, size of sprite,colour mode,pallate,xloc,yloc)
	puck = new GameObject();
	puck->spriteIndex=1;
	puck->width=puck->height=30;
	puck->radius=15;
	puck->speed=0;
	PA_DualCreateSprite(1,(void*)puck_image_Sprite, OBJ_SIZE_32X32,1, 5, SWIDTH/2-puck->radius, ((u32)((SHEIGHT*1.5)+SCREENHOLE))-puck->radius); 

	// This'll be the computer (only visable on top screen)
	computerHandle = new GameObject(); 	
	computerHandle->spriteIndex=2;
	computerHandle->width=handle->height=computerWidth;
	computerHandle->radius=computerWidth/2;
	computerHandle->speed=0;
	PA_DualCreateSprite(2,computerImage,0,obj_size,1, currentLevel, SWIDTH/2-computerHandle->radius,BORDER+computerHandle->radius);
	
	//Load goals (bottom then top)
	PA_CreateSprite(0,3,(void*)goal_bottom_left_Sprite, OBJ_SIZE_64X64,1, 7, SWIDTH/2-64,SHEIGHT-64);
	PA_CreateSprite(0,4,(void*)goal_bottom_right_Sprite, OBJ_SIZE_64X64,1, 7, SWIDTH/2,SHEIGHT-64);
	PA_CreateSprite(1,3,(void*)goal_top_left_Sprite, OBJ_SIZE_64X64,1, 7, SWIDTH/2-64,0);
	PA_CreateSprite(1,4,(void*)goal_top_right_Sprite, OBJ_SIZE_64X64,1, 7, SWIDTH/2,0);
	
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

	//Fill up numbers array
	allNumbers[0] = (void*)number0_Sprite;
	allNumbers[1] = (void*)number1_Sprite;
	allNumbers[2] = (void*)number2_Sprite;
	allNumbers[3] = (void*)number3_Sprite;
	allNumbers[4] = (void*)number4_Sprite;
	allNumbers[5] = (void*)number5_Sprite;
	allNumbers[6] = (void*)number6_Sprite;
	allNumbers[7] = (void*)number7_Sprite;
	allNumbers[8] = (void*)number8_Sprite;
	allNumbers[9] = (void*)number9_Sprite;


	//Reset game objects
	reset();
}

void InGame::resetGameObjects(){
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
	computerHandle->y = BORDER+handle->radius+computerHandle->radius<<8;
	computerHandle->vx = 0;
	computerHandle->vy = 0;
	computerHandle->speed = 0;
}
/**
**InGame reset whole state
**/
void InGame::reset(){
	resetGameObjects();
	playerScore =0;
	computerScore =0;
}
/**
**InGame run
**/
void InGame::run(){
	while(inGame)
	{
		//Process input
		processInput();
		
		//Do intel
		(this->*intelPointer)();

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
	handle->speed = (handle->speed>20)? 20:handle->speed;

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
Do intel function atm very arbitray movement, should really be treated as
game object with proper speed and trajectory, and then updated accordingly
however we are trying to keep this as simple as possible!
Originally it was concieved that each levels AI could behave completly differently
(hence function for each level with pointer)
however it ended up easier just to adjust simple variables, meaning it may have been
better design to have just included this values in a simple AI object and have had 
a singular do AI routine.
**/

void InGame::doIntel1(){
	computerHandle->speed=2;
	
	static int waiting=0;
	int waitingPeriod = 120;
	static int timeSpentWaiting=0;


	//Only react if puc is on our screen
	if(puck->getY()<SHEIGHT&&puck->getX()>computerHandle->radius+16&&puck->getX()<SWIDTH-computerHandle->radius-16){
		if(waiting){
			timeSpentWaiting++;
			if(timeSpentWaiting>=waitingPeriod){
				waiting=0;
				timeSpentWaiting=0;
			}
			return;
		}

		if(PA_RandMinMax(1,600)==1){
			waiting = 1;
		}
		
		if(computerHandle->x>puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x-=((1<<8)/2);
		}
		else if(computerHandle->x<puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x+=((1<<8)/2);
		}
		if(puck->angle==0||puck->angle==256){computerHandle->y-=((1<<8)/30);}
		else if(computerHandle->getY()<BORDER+handle->radius+computerHandle->radius){computerHandle->y+=((1<<8)/30);}
	}
}
void InGame::doIntel2(){
	computerHandle->speed=4;

	static int waiting=0;
	int waitingPeriod = 60;
	static int timeSpentWaiting=0;


	//Only react if puc is on our screen
	if(puck->getY()<SHEIGHT&&puck->getX()>computerHandle->radius+16&&puck->getX()<SWIDTH-computerHandle->radius-16){
		if(waiting){
			timeSpentWaiting++;
			if(timeSpentWaiting==waitingPeriod){
				waiting=0;
				timeSpentWaiting=0;
			}
			return;
		}

		if(PA_RandMinMax(1,1000)==1){
			waiting = 1;
		}
		if(computerHandle->x>puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x-=((1<<8));
		}
		else if(computerHandle->x<puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x+=((1<<8));
		}
		if(puck->angle==0||puck->angle==256){computerHandle->y-=((1<<8)/30);}
		else if(computerHandle->getY()<BORDER+handle->radius+computerHandle->radius){computerHandle->y+=((1<<8)/30);}
	}
}
void InGame::doIntel3(){
	computerHandle->speed=12;

	static int waiting=0;
	int waitingPeriod = 60;
	static int timeSpentWaiting=0;


	//Only react if puc is on our screen
	if(puck->getY()<SHEIGHT&&puck->getX()>computerHandle->radius+16&&puck->getX()<SWIDTH-computerHandle->radius-16){
		if(waiting){
			timeSpentWaiting++;
			if(timeSpentWaiting==waitingPeriod){
				waiting=0;
				timeSpentWaiting=0;
			}
			return;
		}

		if(PA_RandMinMax(1,2000)==1){
			waiting = 1;
		}
		if(computerHandle->x>puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x-=((1<<8));
		}
		else if(computerHandle->x<puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x+=((1<<8));
		}
		if(puck->angle==0||puck->angle==256){computerHandle->y-=((1<<8)/30);}
		else if(computerHandle->getY()<BORDER+handle->radius+computerHandle->radius){computerHandle->y+=((1<<8)/30);}
	}
}
void InGame::doIntel4(){
	computerHandle->speed=12;

	//Only react if puc is on our screen
	if(puck->getY()<SHEIGHT&&puck->getX()>computerHandle->radius+16&&puck->getX()<SWIDTH-computerHandle->radius-16){
		if(computerHandle->x>puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x-=((s32)((1<<8)/2*1.5));
		}
		else if(computerHandle->x<puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x+=((s32)((1<<8)/2*1.5));
		}
		if(puck->angle==0||puck->angle==256){computerHandle->y-=((1<<8)/30);}
		else if(computerHandle->getY()<BORDER+handle->radius+computerHandle->radius){computerHandle->y+=((1<<8)/30);}
	}
}
void InGame::doIntel5(){
	computerHandle->speed=14;

	//Only react if puc is on our screen
	if(puck->getY()<SHEIGHT&&puck->getX()>computerHandle->radius+16&&puck->getX()<SWIDTH-computerHandle->radius-16){
		if(computerHandle->x>puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x-=((1<<8));
		}
		else if(computerHandle->x<puck->x&&puck->y>computerHandle->y+2){
			computerHandle->x+=((1<<8));
		}
		if(puck->angle==0||puck->angle==256){computerHandle->y-=((1<<8)/30);}
		else if(computerHandle->getY()<BORDER+handle->radius+computerHandle->radius){computerHandle->y+=((1<<8)/30);}
	}

}
/**
Do drawing function
**/
void InGame::doDrawing(void){
	// Since collision is done between top right of one object and centre of moving puc must offset by half sprite size (16px)
	
	//This draws the puck
	PA_DualSetSpriteXY(puck->spriteIndex, puck->getX()-puck->radius, puck->getY()-puck->radius);
	
	//Draw the handle
	PA_DualSetSpriteXY(handle->spriteIndex,handle->getX()-handle->radius,handle->getY()-handle->radius);
	
	//Draw computer
	PA_DualSetSpriteXY(computerHandle->spriteIndex,computerHandle->getX()-computerHandle->radius,computerHandle->getY()-computerHandle->radius);
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
				goalScored(0);
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
				goalScored(1);
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
	//PA_ClearTextBg(1);

	//Get the square of the max distance
	u16 maxDistance = (puck->radius+handle->radius)*(puck->radius+handle->radius);
	// Collision between puck and handle note PA-DISTANCE returns the distance squared 
	if (PA_Distance(handle->getX(), handle->getY(), puck->getX(), puck->getY()) < maxDistance) {
		
		//Play sound
		uint16 totalSpeed = puck->speed + handle->speed;
		uint16 volume = (uint16)((127/40)*totalSpeed);

		PA_PlaySound(0,puckhandlecollision,(s32)puckhandlecollision_size,volume,11025);

		u32 overlapDistance = ((puck->radius+handle->radius)-getDistance(handle->getX(), handle->getY(), puck->getX(), puck->getY()));
		
		u16 angle = PA_GetAngle(handle->getX(), handle->getY(), puck->getX(), puck->getY()); 

		if(puck->speed>0){puck->speed-=2;}
		if(handle->speed>puck->speed){puck->speed=handle->speed;}
		if(puck->speed>20){puck->speed=20;}

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
	PA_ClearTextBg(1);

	//Put your debug print statements here.... make sure to print to screen 1
}

/**
**Function that gets called when someone scores
**/
void InGame::goalScored(int computerScored){
	resetGameObjects();
	doDrawing();
	goal=0;
	hooked =0;
	
	if(computerScored){
		PA_PlaySimpleSound(0, computer_score);
		computerScore++;
		drawScore(1);
		if(computerScore==5){
			currentLevelSplash->setLevel(6);
			inGame=0;
			delete mainState;
			mainState = currentLevelSplash;
			PA_WaitForVBL();
		}
	}
	else{
		PA_PlaySimpleSound(0, player_score);
		playerScore++;
		drawScore(0);
		if(playerScore==5){
			currentLevelSplash->setLevel(currentLevel+1);
			inGame=0;
			delete mainState;
			mainState = currentLevelSplash;
			PA_WaitForVBL();
		}
	}
}

void InGame::drawScore(int screen){
	
	uint16 startx=64;
	uint16 starty=60;
	int i=0;
	
	//Goal! sprite
	PA_CreateSprite(screen,5,(void*)goal1_Sprite,OBJ_SIZE_32X32,1,8,startx, starty);
	PA_CreateSprite(screen,6,(void*)goal2_Sprite,OBJ_SIZE_32X32,1,8,startx+32, starty);
	PA_CreateSprite(screen,7,(void*)goal3_Sprite,OBJ_SIZE_32X32,1,8,startx+64, starty);
	PA_CreateSprite(screen,8,(void*)goal4_Sprite,OBJ_SIZE_32X32,1,8,startx+96, starty);
	
	starty+=32;

	//Scoreboard
	PA_CreateSprite(screen,9,(void*)scoreboard1_Sprite,OBJ_SIZE_32X32,1,8,startx, starty);
	PA_CreateSprite(screen,10,(void*)scoreboard2_Sprite,OBJ_SIZE_32X32,1,8,startx+32, starty);
	PA_CreateSprite(screen,11,(void*)scoreboard3_Sprite,OBJ_SIZE_32X32,1,8,startx+64, starty);
	PA_CreateSprite(screen,12,(void*)scoreboard4_Sprite,OBJ_SIZE_32X32,1,8,startx+96, starty);
	
	startx+=20;
	//Draw Score
	PA_CreateSprite(screen,13,allNumbers[playerScore],OBJ_SIZE_32X32,1,8,startx, starty);
	PA_CreateSprite(screen,14,allNumbers[computerScore],OBJ_SIZE_32X32,1,8,startx+57, starty);
	
	
	for (i = 5; i < 15; i++) {
		PA_SetSpriteExtPrio(screen, i, 15-i);
	}	

	for (i = 0; i <= 120; i++) {
		PA_WaitForVBL();
	}	
	
	for (i = 5; i < 15; i++) {
		PA_DeleteSprite(screen, i);
	}
}
