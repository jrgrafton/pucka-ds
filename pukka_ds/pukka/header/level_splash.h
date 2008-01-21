#ifndef __LEVELSPLASH1__
#define __LEVELSPLASH1__

class LevelSplash : public State{
	public:
		LevelSplash(); //Level that this splash is for
		~LevelSplash();
		void run();
		void reset();
		NAME getMyName(); //Not implemented yet
		void increaseLevel();
		int currentLevel;
	protected:
		void init();
	private:
		const void* allImages[5];
		
};
#endif

