//Internal headers
#include "../header/main.h"
#include "../header/state.h"
#include "../header/level_splash.h"
#include "../header/in_game.h"

#include "char1_image.h"
#include "char2_image.h"
#include "char3_image.h"
#include "char4_image.h"
#include "char5_image.h"

/**
**LevelSplash constructor
**/
LevelSplash::LevelSplash() : State(){
	init();
}

/**
**LevelSplash destructor
**/
LevelSplash::~LevelSplash(){

}

/**
**LevelSplash run function
**/
void LevelSplash::run(){
	PA_Init8bitBg(0,0); //Init backgrounds
	PA_Init8bitBg(1,0);

	PA_LoadGif(	0, (void*)allImages[currentLevel]); // Gif File

	for(uint i = -5; i <= 0; i++){
		PA_WaitForVBL();		   
	}  

	while(!Stylus.Newpress){ // Display until keys pressed
		PA_WaitForVBL();
	}
	PA_ResetBgSys();
	PA_ClearTextBg (0);

	//Switch to main game state (needs to be modified to menu state)
	mainState = new InGame(currentLevel);
}
/**
**LevelSplash init function
**/
void LevelSplash::init(){
	myName = LEVELSPLASH;
	currentLevel = 0;

	allImages[0] = char1_image;
	allImages[1] = char2_image;
	allImages[2] = char3_image;
	allImages[3] = char4_image;
	allImages[4] = char5_image;
}
void LevelSplash::increaseLevel(){
	currentLevel++;
}

/**
**LevelSplash reset function
**/
void LevelSplash::reset(){
	currentLevel=0;
}

