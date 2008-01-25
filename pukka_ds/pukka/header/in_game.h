#ifndef __INGAME1__
#define __INGAME1__

#include "../header/game_object.h"

//Main State constants
const u16 SCREENHOLE =48;	//How big percieved hole between screens is
const u16 BORDER = 4;	//Border in pixels around screen (used for collisions)

//Defines for width and height of screen
const u16 SHEIGHT = 191;
const u16 SWIDTH = 255;

//Defines for width of pitch
const u16 PWIDTH = SWIDTH-(BORDER*2);
const u16 PHEIGHT = SHEIGHT-BORDER;//No border on top

//Define max min x for game objects
const u16 MAXX = SWIDTH - BORDER;
const u16 MINX = 0+BORDER;

//Defines for max min y for an game object to be screen 0 (bottom)
const u16 MAXYS0 = SHEIGHT*2+SCREENHOLE - BORDER;
const u16 MINYS0 = 0+SHEIGHT+SCREENHOLE;

//Defines max min y for game object to be on screen 1 (top)
const u16 MAXYS1 = SHEIGHT;
const u16 MINYS1 = 0+BORDER;

//Defines max min y for game object to be dual screen
const u16 MAXYDUAL = SHEIGHT*2+SCREENHOLE - BORDER;
const u16 MINYDUAL = 0+BORDER;

//Define for square root LUT size
const u16 LUTSIZE = 4624;

//Define for goal
const u16 GOALWIDTH = 80;
const u16 GOALHEIGHT = 10;

class InGame : public State{
	public:
		InGame(int level);
		~InGame();
		void run();
		void reset();
		void initGraphics();
		NAME getMyName(); //Not implemented yet
	protected:
		void init(int level);
	private:
		int playerScore;
		int computerScore;

		void resetGameObjects();
		void doCollisions(void);
		u32 getDistance(s16 x1,s16 y1,s16 x2,s16 y2);
		void doDrawing(void);
		void print_debug(void);
		void processInput(void);
		inline u32 square(u32 a);
		void goalScored();
		void doIntel1();
		void doIntel2();
		void doIntel3();
		void doIntel4();
		void doIntel5();
		void handlePuckCollision(GameObject * puck,GameObject * handle);
		void boundaryCheck(GameObject * gameObject,s32 minx,s32 maxx,s32 miny,s32 maxy);
		void boundaryCheckPuck();
		
		//Level dependent stuff
		int currentLevel;
		int computerWidth;
		void (InGame::* intelPointer) ();	    //What AI are we gonna use?
		void* computerImage;		//What computer image are we going to use
		int obj_size;

};
#endif
