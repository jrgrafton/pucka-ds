//Internal headers
#include "../header/main.h"
#include "../header/state.h"
#include "../header/menu.h"
#include "../header/level_splash.h"
#include "main_image.h"
#include "music.h"  // Include the sound (found in the data folder in .raw format)


/**
**Splash constructor
**/
Menu::Menu() : State(){
	init();
	myName = MAINMENU;
}

/**
**Splash destructor
**/
Menu::~Menu(){

}

/**
**Splash run function
**/
void Menu::run(){
    
	while(!Stylus.Newpress){ // Display until keys pressed
		PA_WaitForVBL();
	}
	PA_ResetBgSys();
	PA_WaitForVBL();

	//Switch to main game state (needs to be modified to menu state)
	delete mainState;
	mainState = currentLevelSplash;
}
/**
**Splash init function
**/
void Menu::init(){
	PA_Init8bitBg(0,0); //Init backgrounds
	PA_Init8bitBg(1,0);
	
	PA_LoadGif(	0, (void*)main_image); // Gif File
	PA_PlaySoundRepeat(0, music);
	
	//Give backgrounds time to load
	for(uint i = -5; i <= 0; i++){
		PA_WaitForVBL();		   
	}  

}

/**
**Splash reset function
**/
void Menu::reset(){}

