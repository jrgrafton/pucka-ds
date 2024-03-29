//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif

// Sprite files : 
extern const unsigned char circle_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : sprite0_Pal
extern const unsigned char handle_image_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : handle_image_Pal
extern const unsigned char puck_image_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : puck_image_Pal
extern const unsigned char goal_bottom_left_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : goal_Pal
extern const unsigned char goal_bottom_right_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : goal_Pal
extern const unsigned char goal_top_left_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : goal_Pal
extern const unsigned char goal_top_right_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : goal_Pal
extern const unsigned char char1_image_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : char1_image_Pal
extern const unsigned char char2_image_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : char2_image_Pal
extern const unsigned char char3_image_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : char3_image_Pal
extern const unsigned char char4_image_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : char4_image_Pal
extern const unsigned char char5_image_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : char5_image_Pal
extern const unsigned char scoreboard1_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char scoreboard2_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char scoreboard3_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char scoreboard4_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char goal1_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char goal2_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char goal3_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char goal4_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number0_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number1_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number2_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number3_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number4_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number5_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number6_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number7_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number8_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal
extern const unsigned char number9_Sprite[1024] __attribute__ ((aligned (4))) ;  // Pal : goal_palette_Pal

// Background files : 
extern const int table_top_Info[3]; // BgMode, Width, Height
extern const unsigned short table_top_Map[768] __attribute__ ((aligned (4))) ;  // Pal : table_top_Pal
extern const unsigned char table_top_Tiles[49152] __attribute__ ((aligned (4))) ;  // Pal : table_top_Pal
extern PAGfx_struct table_top; // background pointer

extern const int table_bottom_Info[3]; // BgMode, Width, Height
extern const unsigned short table_bottom_Map[768] __attribute__ ((aligned (4))) ;  // Pal : table_bottom_Pal
extern const unsigned char table_bottom_Tiles[48960] __attribute__ ((aligned (4))) ;  // Pal : table_bottom_Pal
extern PAGfx_struct table_bottom; // background pointer

// Palette files : 
extern const unsigned short sprite0_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short table_top_Pal[92] __attribute__ ((aligned (4))) ;
extern const unsigned short puck_image_Pal[27] __attribute__ ((aligned (4))) ;
extern const unsigned short handle_image_Pal[167] __attribute__ ((aligned (4))) ;
extern const unsigned short table_bottom_Pal[116] __attribute__ ((aligned (4))) ;
extern const unsigned short goal_Pal[87] __attribute__ ((aligned (4))) ;
extern const unsigned short char1_image_Pal[215] __attribute__ ((aligned (4))) ;
extern const unsigned short char2_image_Pal[225] __attribute__ ((aligned (4))) ;
extern const unsigned short char3_image_Pal[233] __attribute__ ((aligned (4))) ;
extern const unsigned short char4_image_Pal[232] __attribute__ ((aligned (4))) ;
extern const unsigned short char5_image_Pal[238] __attribute__ ((aligned (4))) ;
extern const unsigned short goal_score_Pal[116] __attribute__ ((aligned (4))) ;
#endif

