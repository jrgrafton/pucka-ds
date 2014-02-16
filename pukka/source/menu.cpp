//Internal headers
#include "../header/main.h"
#include "../header/state.h"
#include "../header/menu.h"
#include "../header/level_splash.h"
#include "main_image.h"
#include "main_music.h"  // Include the sound (found in the data folder in .raw format)


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
	s8 i;

	for (i = TRANS_LENGTH; i >= 0; i--) { // Fade length...
		PA_BgTransCenter(0, // screen
					  2, // fade type, from 0 to 4, test them out !				  
					  0, // invert
					  i); // Time, 0 being the screen completely visible, 32 completely out
		PA_WaitForVBL(); // To slow down the fades, we wait a frame...
	}
	PA_PlaySoundRepeat(0, main_music);
	while(!Stylus.Newpress){ // Display until keys pressed
		PA_WaitForVBL();
	}
	
	for (i = 0; i <= TRANS_LENGTH; i++) {
		PA_BgTransCenter(0, 2, 0, i);// same thing...
		PA_WaitForVBL(); // To slow down the fades, we wait a frame...
	}	

	PA_ResetBgSys();
	PA_WaitForVBL();

	//Switch to level splash
	delete mainState;
	mainState = currentLevelSplash;
}
/**
**Splash init function
**/
void Menu::init(){
	PA_Init8bitBg(0,0); //Init backgrounds
	PA_InitBgTrans(0);
	
	PA_LoadGif(	0, (void*)main_image); // Gif File
	
	//Give backgrounds time to load
	for(uint i = -5; i <= 0; i++){
		PA_WaitForVBL();		   
	}  

}

/**
**Splash reset function
**/
void Menu::reset(){}

