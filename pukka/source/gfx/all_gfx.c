//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifndef ALL_GFX_C
#define ALL_GFX_C

#ifdef __cplusplus
extern "C" {
#endif


// Sprite files : 
#include "circle.c"
#include "table_top.c"
#include "table_bottom.c"
#include "handle_image.c"
#include "puck_image.c"
#include "goal_bottom_left.c"
#include "goal_bottom_right.c"
#include "goal_top_left.c"
#include "goal_top_right.c"
#include "char1_image.c"
#include "char2_image.c"
#include "char3_image.c"
#include "char4_image.c"
#include "char5_image.c"
#include "scoreboard1.c"
#include "scoreboard2.c"
#include "scoreboard3.c"
#include "scoreboard4.c"
#include "goal1.c"
#include "goal2.c"
#include "goal3.c"
#include "goal4.c"
#include "number0.c"
#include "number1.c"
#include "number2.c"
#include "number3.c"
#include "number4.c"
#include "number5.c"
#include "number6.c"
#include "number7.c"
#include "number8.c"
#include "number9.c"

// Palette files : 
#include "sprite0.pal.c"
#include "table_top.pal.c"
#include "table_bottom.pal.c"
#include "handle_image.pal.c"
#include "puck_image.pal.c"
#include "goal.pal.c"
#include "char1_image.pal.c"
#include "char2_image.pal.c"
#include "char3_image.pal.c"
#include "char4_image.pal.c"
#include "char5_image.pal.c"
#include "goal_score.pal.c"

// Background Pointers :
PAGfx_struct table_top = {(void*)table_top_Map, 768, (void*)table_top_Tiles, 49152, (void*)table_top_Pal, (int*)table_top_Info };
PAGfx_struct table_bottom = {(void*)table_bottom_Map, 768, (void*)table_bottom_Tiles, 48960, (void*)table_bottom_Pal, (int*)table_bottom_Info };

#ifdef __cplusplus
}
#endif

#endif

