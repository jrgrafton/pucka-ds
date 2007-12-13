#ifndef __INGAME1__
#define __INGAME1__

//Main State constants
const u16 SCREENHOLE =48;	//How big percieved hole between screens is
const u16 BORDER = 4;	//Border in pixels around screen (used for collisions)

//Defines for width and height of screen
const u16 SHEIGHT = 191;
const u16 SWIDTH = 255;

//Defines for width of pitch
const u16 PWIDTH = SWIDTH-(BORDER*2);
const u16 PHEIGHT = SHEIGHT-BORDER;//No border on top

//Defines for max x and y for an entity to be
const u16 MAXX = SWIDTH - BORDER;
const u16 MAXY = SHEIGHT - BORDER;
const u16 MINX = 0+BORDER;
const u16 MINY = 0+BORDER;

//Define for square root LUT size
const u16 LUTSIZE = 1156;

//Define for goal
const u16 GOALWIDTH = 100;
const u16 GOALHEIGHT = 7;

class InGame : public State{
	public:
		InGame();
		~InGame();
		void run();
		void reset();
		NAME getMyName();
	protected:
		void init();
	private:
		void doCollisions(void);
		u32 getDistance(s16 x1,s16 y1,s16 x2,s16 y2);
		void doDrawing(void);
		void print_debug(void);
		void processInput(void);
		inline u32 square(u32 a);
		void goalScored();
};
#endif
