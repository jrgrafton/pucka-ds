#ifndef __LEVELSPLASH1__
#define __LEVELSPLASH1__

class LevelSplash : public State{
	public:
		LevelSplash(); //Level that this splash is for
		~LevelSplash();
		void run();
		void reset();
		NAME getMyName(); //Not implemented yet
		void setLevel(int level);
		int currentLevel;
	protected:
		void init();
	private:
		void* allImages[7];
		
};
#endif

