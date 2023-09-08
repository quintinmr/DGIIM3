#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>



// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
/** Think tras devolver plan **/
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	// Incluir aquí el comportamiento del agente jugador
	if (!hayPlan){
		// Invocar al método de búsqueda
		cout << "Calculando plan..." << endl;
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula = sensores.sentido;
		c_state.sonambulo.f = sensores.SONposF;
		c_state.sonambulo.c = sensores.SONposC;
		c_state.sonambulo.brujula = sensores.SONsentido;
		c_state.bikini_ON = false;
		c_state.zapatillas_ON = false;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;

		switch(sensores.nivel){

			case 0:

				hayPlan = AnchuraSoloJugador(c_state, goal, plan, mapaResultado);
				break;

			case 1:
				hayPlan = AnchuraSoloJugadorN1(c_state, goal, plan, mapaResultado);
				break;

			case 2:
				hayPlan = DijkstraJugador(c_state, goal, plan ,mapaResultado);
				break;

			case 3:
				hayPlan = AEstrellaSonambulo(c_state, goal, plan ,mapaResultado);
			break;
		}

		if (hayPlan) VisualizaPlan(c_state,plan);
				
			
	}

	if (hayPlan and plan.size()>0){
		cout << "Ejecutando siguiente acción del plan" << endl;
		accion = plan.front();
		plan.pop_front();
	}

	if (plan.size()== 0){
		cout << "Se completó el plan" << endl;
		hayPlan = false;
	}

	return accion;
}

/************************************************************************************************************************************
 * 
 * 	NIVEL 0
 * 
*************************************************************************************************************************************/
bool ComportamientoJugador::AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	current_node.st = inicio;
	list<nodeN0> frontier;
	set <nodeN0> explored;
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);

		// Generar hijo actFORWARD
		nodeN0 child_forward = current_node; // !!!!
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
		if (child_forward.st.jugador.f == final.f and child_forward.st.jugador.c == final.c)
		{
			child_forward.secuencia.push_back(actFORWARD);  // !!!!
			current_node = child_forward;
			SolutionFound = true;
		}
		else if (explored.find(child_forward) == explored.end())
		{
			child_forward.secuencia.push_back(actFORWARD);
			frontier.push_back(child_forward);
		}

		if (!SolutionFound)
		{
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;  // !!!!
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl) == explored.end())
			{
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN0 child_turnr = current_node; // !!!!
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr) == explored.end())
			{
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
		}

		if (!SolutionFound and !frontier.empty())
		{
			current_node = frontier.front();
			while (!frontier.empty() and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				current_node = frontier.front();
			}
		}
	}

	if(SolutionFound){  // !!!!
		plan = current_node.secuencia;
	}

	return SolutionFound;
}


/************************************************************************************************************************************
 * 
 * 	NIVEL 1
 * 
*************************************************************************************************************************************/
bool ComportamientoJugador::AnchuraSoloJugadorN1(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 current_node;
	current_node.st = inicio;
	list<nodeN0> frontier;
	set <nodeN0> explored;
	bool SolutionFound = (current_node.st.sonambulo.f == final.f and current_node.st.sonambulo.c == final.c);
	frontier.push_back(current_node);

	while (!frontier.empty() and !SolutionFound)
	{
		frontier.pop_front();
		explored.insert(current_node);

		if (VeoSonambulo(current_node.st)){  // Si el sonámbulo está en mi campo visual
			
			// Generar hijo actSON_FORWARD del sonámbulo
			nodeN0 child_forwardS = current_node; // !!!!
			child_forwardS.st = apply(actSON_FORWARD, current_node.st, mapa);
			if (child_forwardS.st.sonambulo.f == final.f and child_forwardS.st.sonambulo.c == final.c)
			{
				child_forwardS.secuencia.push_back(actSON_FORWARD);  // !!!!
				current_node = child_forwardS;
				SolutionFound = true;
			}
			else if (explored.find(child_forwardS) == explored.end())
			{
				child_forwardS.secuencia.push_back(actSON_FORWARD);
				frontier.push_back(child_forwardS);
			}

			if (!SolutionFound)
			{
				// Generar hijo actSON_TURN_SL del sonámbulo (giro 45º a la izda sonámbulo)
				nodeN0 child_turnlS = current_node; 
				child_turnlS.st = apply(actSON_TURN_SL, current_node.st, mapa);
				if (explored.find(child_turnlS) == explored.end())
				{
					child_turnlS.secuencia.push_back(actSON_TURN_SL);
					frontier.push_back(child_turnlS);
				}
		
				// Generar hijo actSON_TURN_SR del sonámbulo (giro 45º a la dcha sonámbulo)
				nodeN0 child_turnrS = current_node; 
				child_turnrS.st = apply(actSON_TURN_SR, current_node.st, mapa);
				if (explored.find(child_turnrS) == explored.end())
				{
					child_turnrS.secuencia.push_back(actSON_TURN_SR);
					frontier.push_back(child_turnrS);
				}
			
			}

		}
		else if (!SolutionFound){   // Si no veo al sonámbulo, sigo con mi plan de acción hasta que lo tenga en mi campo visual.

			// Generar hijo actFORWARD
			nodeN0 child_forward = current_node; // !!!!
			child_forward.st = apply(actFORWARD, current_node.st, mapa);
			if (explored.find(child_forward) == explored.end())
			{
				child_forward.secuencia.push_back(actFORWARD);
				frontier.push_back(child_forward);
			}

			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node;  // !!!!
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_turnl) == explored.end())
			{
				child_turnl.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turnl);
			}
			// Generar hijo actTURN_R
			nodeN0 child_turnr = current_node; // !!!!
			child_turnr.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_turnr) == explored.end())
			{
				child_turnr.secuencia.push_back(actTURN_R);
				frontier.push_back(child_turnr);
			}
			

		}

		if (!SolutionFound and !frontier.empty())
		{
			current_node = frontier.front();
			while (!frontier.empty() and explored.find(current_node) != explored.end()){
				frontier.pop_front();
				if (!frontier.empty()) current_node = frontier.front();
			}
		}
	}

	if(SolutionFound){  
		plan = current_node.secuencia;
	}

	return SolutionFound;
}

/** Devuelve si una ubicación en el mapa es transitable para el agente */
bool ComportamientoJugador::CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa)
{
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
}

/** Devuelve la ubicación siguiente según el avance del agente*/
ubicacion ComportamientoJugador::NextCasilla(const ubicacion &pos)
{
	ubicacion next = pos;
	switch (pos.brujula)
	{
	case norte:
		next.f = pos.f - 1;
		break;
	case noreste:
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	case este:
		next.c = pos.c + 1;
		break;
	case sureste:
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	case sur:
		next.f = pos.f + 1;
		break;
	case suroeste:
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	case oeste:
		next.c = pos.c - 1;
		break;
	case noroeste:
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	default:
		break;
	}

	return next;
}

/** Devuelve el estado que se genera si el agente puede avanzar.
 * Si no puede avanzar, se devuelve como salida el mismo estado de entrada.
 */
stateN0 ComportamientoJugador::apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa)
{
  stateN0 st_result = st;
  ubicacion sig_ubicacion;
  switch (a){
    case actFORWARD: // si casilla delante es transitable y no está ocupada por el sonámbulo
      sig_ubicacion = NextCasilla(st.jugador);
      if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.sonambulo.f and sig_ubicacion.c == st.sonambulo.c)){
        st_result.jugador = sig_ubicacion;
      }
      break;
	case actSON_FORWARD:
      sig_ubicacion = NextCasilla(st.sonambulo);
      if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f and sig_ubicacion.c == st.jugador.c)){
        st_result.sonambulo = sig_ubicacion;
      }
      break;
    case actTURN_L:
      st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6) % 8);
      break;

	case actSON_TURN_SL:
      st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 7) % 8);
      break;

    case actTURN_R:
      st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 2) % 8);
      break;

	case actSON_TURN_SR:
      st_result.sonambulo.brujula = static_cast<Orientacion>((st_result.sonambulo.brujula + 1) % 8);
      break;
  }
  return st_result;
}

/** Encuentra si el elmento item está en lista */
bool ComportamientoJugador::Find(const stateN0 &item, const list<nodeN0> &lista)
{
  auto it = lista.begin();
  while (it != lista.end() and !((it->st) == item))
    it++;

  return (!(it == lista.end()));
}

/** pone a cero todos los elementos de una matriz */
void ComportamientoJugador::AnularMatriz(vector<vector<unsigned char>> &matriz)
{
  for (int i = 0; i < matriz.size(); i++)
    for (int j = 0; j < matriz[0].size(); j++)
      matriz[i][j] = 0;
}


/************************************************************************************************************************************
 * 
 * 	NIVEL 2
 * 
*************************************************************************************************************************************/
bool ComportamientoJugador::DijkstraJugador(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa){

	nodeN2 current_node;
	current_node.st = inicio;
	current_node.coste = 0;
 	priority_queue<nodeN2, vector<nodeN2>, nodeN2compare> frontier; // Cola con prioridad para los abiertos
	set <stateN0, statecompare> explored;  // Conjunto ordenado para los cerrados
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);
	frontier.push(current_node);

	while (!frontier.empty() and !SolutionFound){

		frontier.pop();
		if (explored.find(current_node.st) == explored.end()) explored.insert(current_node.st); // lo metemos en cerrados
		actualizarNodo(current_node.st);
		
		// Generar hijo actTurn_L
		nodeN2 child_actTurnL = current_node;
		child_actTurnL.coste += costeDeBateria(mapaResultado[current_node.st.jugador.f][current_node.st.jugador.c], current_node.st.bikini_ON, current_node.st.zapatillas_ON, actTURN_L);
		child_actTurnL.st = apply(actTURN_L, current_node.st, mapa);
		if (explored.find(child_actTurnL.st) == explored.end())
		{
			child_actTurnL.secuencia.push_back(actTURN_L);
			frontier.push(child_actTurnL);
		}

		// Generar hijo actTurn_R
		nodeN2 child_actTurnR = current_node;
		child_actTurnR.coste += costeDeBateria(mapaResultado[current_node.st.jugador.f][current_node.st.jugador.c], current_node.st.bikini_ON, current_node.st.zapatillas_ON, actTURN_R);
		child_actTurnR.st = apply(actTURN_R, current_node.st, mapa);
		if (explored.find(child_actTurnR.st) == explored.end())
		{
			child_actTurnR.secuencia.push_back(actTURN_R);
			frontier.push(child_actTurnR);
		}

		// Generar hijo actForward
		nodeN2 child_forward = current_node;
		child_forward.coste += costeDeBateria(mapaResultado[current_node.st.jugador.f][current_node.st.jugador.c], current_node.st.bikini_ON, current_node.st.zapatillas_ON, actFORWARD);
		child_forward.st = apply(actFORWARD, current_node.st, mapa);
		
		if (explored.find(child_forward.st) == explored.end())
		{
				child_forward.secuencia.push_back(actFORWARD);
				frontier.push(child_forward);
		}

		if (!SolutionFound and !frontier.empty())
		{
			current_node = frontier.top();
			while (!frontier.empty() and explored.find(current_node.st) != explored.end()){
				actualizarNodo(current_node.st);
				frontier.pop();
				current_node = frontier.top();
			}

			if (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c)
			{
				SolutionFound = true;

			}
		}
		
		
	}

	if(SolutionFound){  
		plan = current_node.secuencia;
	}

	return SolutionFound;

}

/************************************************************************************************************************************
 * 
 * 	NIVEL 3
 * 
*************************************************************************************************************************************/
bool ComportamientoJugador::AEstrellaSonambulo(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa){

	nodeN3 current_node;
	current_node.st = inicio;
	current_node.st.bikiniSON_ON = false;
	current_node.st.zapatillasSON_ON = false;
	current_node.coste = 0;
 	priority_queue<nodeN3, vector<nodeN3>, heuristicacompare> frontier; // Cola con prioridad para los abiertos
	set <stateN0, statecompare> explored;  // Conjunto ordenado para los cerrados
	actualizarNodo(current_node.st);
	bool SolutionFound = (current_node.st.sonambulo.f == final.f and current_node.st.sonambulo.c == final.c);
	frontier.push(current_node);

	while (!frontier.empty() and !SolutionFound){

		frontier.pop();
		if (explored.find(current_node.st) == explored.end()) explored.insert(current_node.st); // lo metemos en cerrados
		actualizarNodo(current_node.st);
		
		if (VeoSonambulo(current_node.st)){  // Si el sonámbulo está en mi campo visual
			// Generar hijo actSON_TurnL
			nodeN3 child_actSON_TurnL = current_node;
			child_actSON_TurnL.coste += costeDeBateria(mapaResultado[current_node.st.sonambulo.f][current_node.st.sonambulo.c], current_node.st.bikiniSON_ON, current_node.st.zapatillasSON_ON, actSON_TURN_SL);
			child_actSON_TurnL.heuristica =distanciaDelMaximo(child_actSON_TurnL.st.sonambulo, final);
			child_actSON_TurnL.costPlusHeu = child_actSON_TurnL.coste + child_actSON_TurnL.heuristica;
			child_actSON_TurnL.st = apply(actSON_TURN_SL, current_node.st, mapa);
			if (explored.find(child_actSON_TurnL.st) == explored.end())
			{
				child_actSON_TurnL.secuencia.push_back(actSON_TURN_SL);
				frontier.push(child_actSON_TurnL);
			}

			// Generar hijo actTurn_R
			nodeN3 child_actSON_TurnR = current_node;
			child_actSON_TurnR.coste += costeDeBateria(mapaResultado[current_node.st.sonambulo.f][current_node.st.sonambulo.c], current_node.st.bikiniSON_ON, current_node.st.zapatillasSON_ON, actSON_TURN_SR);
			child_actSON_TurnR.heuristica =distanciaDelMaximo(child_actSON_TurnR.st.sonambulo, final);
			child_actSON_TurnR.costPlusHeu = child_actSON_TurnR.coste + child_actSON_TurnR.heuristica;
			child_actSON_TurnR.st = apply(actSON_TURN_SR, current_node.st, mapa);
			if (explored.find(child_actSON_TurnR.st) == explored.end())
			{
				child_actSON_TurnR.secuencia.push_back(actSON_TURN_SR);
				frontier.push(child_actSON_TurnR);
			}

			// Generar hijo actForward
			nodeN3 child_SONactFORWARD = current_node;
			child_SONactFORWARD.coste += costeDeBateria(mapaResultado[current_node.st.sonambulo.f][current_node.st.sonambulo.c], current_node.st.bikiniSON_ON, current_node.st.zapatillasSON_ON, actSON_FORWARD);
			child_SONactFORWARD.heuristica =distanciaDelMaximo(child_SONactFORWARD.st.sonambulo, final);
			child_SONactFORWARD.costPlusHeu = child_SONactFORWARD.coste + child_SONactFORWARD.heuristica;
			child_SONactFORWARD.st = apply(actSON_FORWARD, current_node.st, mapa);
			if (explored.find(child_SONactFORWARD.st) == explored.end())
			{
				child_SONactFORWARD.secuencia.push_back(actSON_FORWARD);
				frontier.push(child_SONactFORWARD);
			}

		}

		if (!SolutionFound){
			
			// Generar hijo actTurn_L
			nodeN3 child_actTurnL = current_node;
			child_actTurnL.coste += costeDeBateria(mapaResultado[current_node.st.jugador.f][current_node.st.jugador.c], current_node.st.bikini_ON, current_node.st.zapatillas_ON, actTURN_L);
			child_actTurnL.heuristica =distanciaDelMaximo(child_actTurnL.st.sonambulo, final);
			child_actTurnL.costPlusHeu = child_actTurnL.coste + child_actTurnL.heuristica;
			child_actTurnL.st = apply(actTURN_L, current_node.st, mapa);
			if (explored.find(child_actTurnL.st) == explored.end())
			{
				child_actTurnL.secuencia.push_back(actTURN_L);
				frontier.push(child_actTurnL);
			}

			// Generar hijo actTurn_R
			nodeN3 child_actTurnR = current_node;
			child_actTurnR.coste += costeDeBateria(mapaResultado[current_node.st.jugador.f][current_node.st.jugador.c], current_node.st.bikini_ON, current_node.st.zapatillas_ON, actTURN_R);
			child_actTurnR.heuristica =distanciaDelMaximo(child_actTurnR.st.sonambulo, final);
			child_actTurnR.costPlusHeu = child_actTurnR.coste + child_actTurnR.heuristica;
			child_actTurnR.st = apply(actTURN_R, current_node.st, mapa);
			if (explored.find(child_actTurnR.st) == explored.end())
			{
				child_actTurnR.secuencia.push_back(actTURN_R);
				frontier.push(child_actTurnR);
			}

			// Generar hijo actForward
			nodeN3 child_forward = current_node;
			child_forward.coste += costeDeBateria(mapaResultado[current_node.st.jugador.f][current_node.st.jugador.c], current_node.st.bikini_ON, current_node.st.zapatillas_ON, actFORWARD);
			child_forward.heuristica =distanciaDelMaximo(child_forward.st.sonambulo, final);
			child_forward.costPlusHeu = child_forward.coste + child_forward.heuristica;
			child_forward.st = apply(actFORWARD, current_node.st, mapa);
			
			if (explored.find(child_forward.st) == explored.end())
			{
					child_forward.secuencia.push_back(actFORWARD);
					frontier.push(child_forward);
			}

		}

		if (!SolutionFound and !frontier.empty())
		{
				current_node = frontier.top();
				while (!frontier.empty() and explored.find(current_node.st) != explored.end()){
					actualizarNodo(current_node.st);
					frontier.pop();
					current_node = frontier.top();
				}

				if (current_node.st.sonambulo.f == final.f and current_node.st.sonambulo.c == final.c)
				{
					SolutionFound = true;

				}
		}
		
	}

	if(SolutionFound){  
		plan = current_node.secuencia;
	}

	return SolutionFound;

}



int ComportamientoJugador::costeDeBateria(unsigned char surface, bool bikini_ON, bool zapatillas_ON, Action accion){

	int coste = 1;

	switch(accion){

		case actFORWARD : case actSON_FORWARD:
			
			if (surface == 'A'){
				if (bikini_ON) coste = 10;
				else coste = 100;
			}
			else if (surface == 'B'){
				if (zapatillas_ON) coste = 15;
				else coste = 50;
			}
			else if (surface == 'T') coste = 2;
			else coste = 1;
			break;

		case actTURN_L: case actTURN_R:

			if (surface == 'A'){

				if (bikini_ON) coste = 5;
				else coste = 25;

			}
			else if (surface == 'B'){
				if (zapatillas_ON) coste = 1;
				else coste = 5;
			}
			else if (surface == 'T') coste = 2;
			else coste = 1;

			break;

		case actSON_TURN_SL: case actSON_TURN_SR:

			if (surface == 'A'){

				if (bikini_ON) coste = 2;
				else coste = 7;

			}
			else if (surface == 'B'){
				if (zapatillas_ON) coste = 1;
				else coste = 3;
			}
			else coste = 1;

			break;

		case actIDLE:
			coste = 0;
			break;

		case actWHEREIS:
			coste = 200;
			break;

	}

	return coste;

	
}

void ComportamientoJugador::actualizarNodo(stateN0 &st){

	switch (mapaResultado[st.jugador.f][st.jugador.c]){

		case 'K':
			st.bikini_ON     = true;
			if (st.zapatillas_ON) st.zapatillas_ON = false;
			break;
		case 'D':
			st.zapatillas_ON = true;
			if (st.bikini_ON) st.bikini_ON     = false;
			break;

	}

	switch (mapaResultado[st.sonambulo.f][st.sonambulo.c]){

		case 'K':
			st.bikiniSON_ON     = true;
			if (st.zapatillasSON_ON) st.zapatillasSON_ON = false;
			break;
		case 'D':
			st.zapatillasSON_ON = true;
			if (st.bikiniSON_ON) st.bikiniSON_ON     = false;
			break;

	}

}




/** Permite pintar sobre el mapa del simulador el plan partiendo desde el estado st */
void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
  AnularMatriz(mapaConPlan);
  stateN0 cst = st;

  auto it = plan.begin();
  while (it != plan.end()){
    switch (*it){
      case actFORWARD:
        cst.jugador = NextCasilla(cst.jugador);
	  mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
	  break;
	case actTURN_R:
        cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 2) % 8);
	  break;
	case actTURN_L:
	  cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
	  break;
	case actSON_FORWARD:
	  cst.sonambulo = NextCasilla(cst.sonambulo);
	  mapaConPlan[cst.sonambulo.f][cst.sonambulo.c] = 2;
	  break;
	case actSON_TURN_SR:
	  cst.sonambulo.brujula = (Orientacion)((cst.sonambulo.brujula + 1) % 8);
	  break;
	case actSON_TURN_SL:
	  cst.sonambulo.brujula = (Orientacion)((cst.sonambulo.brujula + 7) % 8);
	  break;
    }
    it++;
  }
}


bool ComportamientoJugador::VeoSonambulo(stateN0 &jug)
{
	
	bool LOVEO = false;

	switch(jug.jugador.brujula){

		case norte:

			if (jug.sonambulo.f == jug.jugador.f-1){
				for (int i = jug.jugador.c-1; i <= jug.jugador.c+1; i++)
					if (jug.sonambulo.c == i) LOVEO = true;
			}
			else if (jug.sonambulo.f == jug.jugador.f-2){
				for (int i = jug.jugador.c-2; i <= jug.jugador.c+2; i++)
					if (jug.sonambulo.c == i) LOVEO = true;
			}
			else if (jug.sonambulo.f == jug.jugador.f-3){
				for (int i = jug.jugador.c-3; i <= jug.jugador.c+3; i++)
					if (jug.sonambulo.c == i) LOVEO = true;
			}

		break;

		case sur:

			if (jug.sonambulo.f == jug.jugador.f + 1){
				for (int i = jug.jugador.c-1; i <= jug.jugador.c+1; i++)
					if (jug.sonambulo.c == i) LOVEO = true;
			}
			else if (jug.sonambulo.f == jug.jugador.f + 2){
				for (int i = jug.jugador.c-2; i <= jug.jugador.c+2; i++)
					if (jug.sonambulo.c == i) LOVEO = true;
			}
			else if (jug.sonambulo.f == jug.jugador.f + 3){
				for (int i = jug.jugador.c-3; i <= jug.jugador.c+3; i++)
					if (jug.sonambulo.c == i) LOVEO = true;
			}
		break;

		case este:

			if (jug.sonambulo.c == jug.jugador.c + 1){
				for (int i = jug.jugador.f-1; i <=jug.jugador.f+1; i++)
					if (jug.sonambulo.f == i) LOVEO = true;
			}
			else if (jug.sonambulo.c == jug.jugador.c + 2){
				for (int i = jug.jugador.f-2; i <= jug.jugador.f+2; i++)
					if (jug.sonambulo.f == i) LOVEO = true;
			}
			else if (jug.sonambulo.c == jug.jugador.c + 3){
				for (int i = jug.jugador.f-3; i <= jug.jugador.f+3; i++)
					if (jug.sonambulo.f == i) LOVEO = true;
			}
		break;

		case oeste:

			if (jug.sonambulo.c == jug.jugador.c - 1){
				for (int i = jug.jugador.f-1; i <= jug.jugador.f+1; i++)
					if (jug.sonambulo.f == i) LOVEO = true;
			}
			else if (jug.sonambulo.c == jug.jugador.c - 2){
				for (int i = jug.jugador.f-2; i <= jug.jugador.f+2; i++)
					if (jug.sonambulo.f == i) LOVEO = true;
			}
			else if (jug.sonambulo.c == jug.jugador.c - 3){
				for (int i = jug.jugador.f-3; i <= jug.jugador.f+3; i++)
					if (jug.sonambulo.f == i) LOVEO = true;
			}
		break;
	}

	return LOVEO;
}
// Distancia del máximo
int ComportamientoJugador::distanciaDelMaximo(const ubicacion &son, const ubicacion &dest)
{
	return (max(abs(son.f-dest.f), abs(son.c-dest.c)));
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
