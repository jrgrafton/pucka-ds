//Custom header includes
#include "../header/main.h"

State * mainState;

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
	mainState = new InGame();
}

/**
**Call this to reset entities
**/
void reset(){
	
}


