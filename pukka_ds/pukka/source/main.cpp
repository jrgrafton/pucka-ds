//Custom header includes
#include "../header/main.h"
#include "../header/state.h"
#include "../header/game_object.h"
#include "../header/in_game.h"
#include "../header/level_splash.h"
#include "../header/splash.h"
#include "../source/gfx/all_gfx.c"

State * mainState;
State * currentLevelSplash;

int main(void){
	//Initialise everything
	init();
	
	while(1)
	{
		mainState->run();
	}
	return 0;
}

/**
Initialise everything
**/
void init(void){
	//Init libs
	PA_Init();
	PA_InitVBL();
	PA_InitSound();  // Init the sound system
	PA_Init8bitBg(0,0); //Init backgrounds
	PA_Init8bitBg(1,0);
	
	//Init text
	PA_InitText(1,0); // On the top screen
	PA_InitText(0,0); // On the bottom screen
	PA_SetTextCol(1,0,0,0);
	PA_SetTextCol(0,31,31,31);
	mainState = new Splash();

	//Keep in game and levelsplash memeory resident
	currentLevelSplash = new LevelSplash();
}

/**
**Call this to reset entities
**/
void reset(){
	
}


