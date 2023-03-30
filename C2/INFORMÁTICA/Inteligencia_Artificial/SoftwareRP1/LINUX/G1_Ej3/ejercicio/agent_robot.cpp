#include "agent_robot.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	ActionType accion = actIDLE;

	/* ESCRIBA AQUI SUS REGLAS */

	if (girando){
		accion = actTURN_L;
		girando = false;
	}
	else if (!CNY70_){
		accion = actFORWARD;
		if (encontrada_frontera == true){
			DIMENSION++;
		}
	}
	else if (CNY70_ and !encontrada_frontera){
		accion = actTURN_L;
		DIMENSION++;
		girando = true;
		encontrada_frontera = true;
	}
	else if(CNY70_ and encontrada_frontera){
		cout << "La dimensión es: " << DIMENSION << endl;
	}
	
	


	return static_cast<ActionType> (accion);

}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	CNY70_ = env.isFrontier();
	BUMPER_ = env.isBump();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actBACKWARD: return "BACKWARD";
	case Agent::actPUSH: return "PUSH";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
