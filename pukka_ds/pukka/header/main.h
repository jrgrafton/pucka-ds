#ifndef __MAIN1__
#define __MAIN1__
// Includes
#include <PA9.h>
#include <math.h>

//GFX Includes
#include "../source/gfx/all_gfx.h"

//Internal include
#include "../header/state.h"

//Const definitions
//#define DEBUG				//Comment out to turn off debug mode
void init(void);
void reset();

//Extern main state so other objects can modify it
extern State * mainState;
//Game and level splash is always memory resident
extern State * currentLevelSplash;


#endif

