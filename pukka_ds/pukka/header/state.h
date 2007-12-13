#ifndef __STATE1__
#define __STATE1__

//State is basically an interface that all other states inherit
class State {
	public:
		//Name of state
		enum NAME { MAINMENU, INGAME};
		NAME myName;
		
		//Internal state that this state currently holds
		State * myState;

		//Set and get internal state
		State * getState();
		void setState(State * state);

		//Get this states name
		NAME getMyName();

		//Reset this state
		virtual void reset() {};

		//Run called by main loop
		virtual void run() {};

	public:
		//Constructor destructor
		State() {};
		virtual ~State() {};
	protected:
		//Allocate resources for this state
		virtual void init() {};
};

//Classes that inherit from main state object
class MainMenu : public State{};

#endif
