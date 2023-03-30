#include "agent_hormiga.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>

using namespace std;

// -----------------------------------------------------------
int contador_signos = 0;

Agent::ActionType Agent::Think()
{
	ActionType accion = actFORWARD;
	
	/* ESCRIBA AQUI LAS REGLAS */
	if (FEROMONA_ == true){

		accion = actFORWARD;

	}
	else if (contador_signos < 1){
		accion = actTURN_R;
		contador_signos++;
	}
	else{
		accion = actTURN_L;
	}
	return static_cast<ActionType> (accion);

}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	FEROMONA_ = env.isFeromona();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actIDLE: return "IDLE";
	default: return "????";
	}
}
