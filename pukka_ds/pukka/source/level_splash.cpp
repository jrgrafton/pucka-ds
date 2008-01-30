//Internal headers
#include "../header/main.h"
#include "../header/menu.h"
#include "../header/state.h"
#include "../header/level_splash.h"
#include "../header/in_game.h"

//Image includes
#include "char1_splash_image.h"
#include "char2_splash_image.h"
#include "char3_splash_image.h"
#include "char4_splash_image.h"
#include "char5_splash_image.h"
#include "win_image.h"
#include "lose_image.h"

//Sound includes
#include "win_music.h"
#include "lose_music.h"
#include "main_music.h"



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

	if(currentLevel==5){
		PA_PlaySimpleSound(0, lose_music);
	}
	else if(currentLevel==6){
		PA_PlaySimpleSound(0, win_music);
	}
	else if(currentLevel>0){
		PA_PlaySoundRepeat(0, main_music);
	}

	PA_LoadGif(	0, allImages[currentLevel]); // Gif File

	for(uint i = -5; i <= 0; i++){
		PA_WaitForVBL();		   
	}  

	while(!Stylus.Newpress){ // Display until keys pressed
		PA_WaitForVBL();
	}
	PA_ResetBgSys();
	PA_StopSound(0);

	//Switch to main game state or menu state
	if(currentLevel==5||currentLevel==6){
		mainState = new Menu();
		currentLevel=0;
		PA_WaitForVBL();
	}
	else{
		mainState = new InGame(currentLevel);
		PA_WaitForVBL();
	}
}
/**
**LevelSplash init function
**/
void LevelSplash::init(){
	myName = LEVELSPLASH;
	currentLevel = 0;

	allImages[0] = (void*)char1_splash_image;
	allImages[1] = (void*)char2_splash_image;
	allImages[2] = (void*)char3_splash_image;
	allImages[3] = (void*)char4_splash_image;
	allImages[4] = (void*)char5_splash_image;
	allImages[5] = (void*)win_image;
	allImages[6] = (void*)lose_image;
}
void LevelSplash::setLevel(int level){
	currentLevel=level;
}

/**
**LevelSplash reset function
**/
void LevelSplash::reset(){
	currentLevel=0;
}

