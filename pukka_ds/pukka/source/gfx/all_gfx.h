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
#endif

