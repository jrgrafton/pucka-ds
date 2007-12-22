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

// Palette files : 
#include "sprite0.pal.c"
#include "table_top.pal.c"
#include "table_bottom.pal.c"
#include "handle_image.pal.c"
#include "puck_image.pal.c"
#include "goal.pal.c"

// Background Pointers :
PAGfx_struct table_top = {(void*)table_top_Map, 768, (void*)table_top_Tiles, 49152, (void*)table_top_Pal, (int*)table_top_Info };
PAGfx_struct table_bottom = {(void*)table_bottom_Map, 768, (void*)table_bottom_Tiles, 48960, (void*)table_bottom_Pal, (int*)table_bottom_Info };

#ifdef __cplusplus
}
#endif

#endif

