#ifndef AGENT__
#define AGENT__

#include <string>
#include <iostream>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
		CNY70_=false;
		BUMPER_=false;
		DIMENSION=0;
		encontrada_frontera=false;
		girando = false;
	}

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
		actBACKWARD,
		actPUSH,
		actIDLE
	};

	void Perceive(const Environment &env);
	ActionType Think();

private:
	bool CNY70_;
	bool BUMPER_;
	int DIMENSION;
	bool encontrada_frontera;
	bool girando;
};

string ActionStr(Agent::ActionType);

#endif
