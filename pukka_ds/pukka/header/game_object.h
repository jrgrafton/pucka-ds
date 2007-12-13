#ifndef __GAME_OBJECT1__
#define __GAME_OBJECT1__

class GameObject{
	public:
		//Fields
		s32 x, y; // point in centre of entity
		s16 vx, vy; // xy trajectory
		s16 speed;
		s16 angle;
		s16 spriteIndex;
		u16 width, height; //Dimensions in pixels
		u16 radius;

		//Functions
		s16 getX();
		s16 getY();
};
#endif
