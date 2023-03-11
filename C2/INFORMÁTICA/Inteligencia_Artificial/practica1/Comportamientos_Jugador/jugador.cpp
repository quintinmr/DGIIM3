#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Noreste" << endl; break;
		case 2: cout << "Este" << endl; break;
		case 3: cout << "Sureste" << endl; break;
		case 4: cout << "Sur " << endl; break;
		case 5: cout << "Suroeste" << endl; break;
		case 6: cout << "Oeste" << endl; break;
		case 7: cout << "Noroeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	// Determinar el efecto de la ultima accion enviada
	switch (last_action){
		case actTURN_SR:
			brujula = static_cast<Orientacion> ((brujula+1)%8);
			break;
		case actTURN_SL:
			brujula = static_cast<Orientacion> ((brujula-1)%8);
			break;
		case actTURN_BL:
			brujula = static_cast<Orientacion> ((brujula-3)%8);
			break;
		case actTURN_BR:
			brujula = static_cast<Orientacion> ((brujula+3)%8);
			break;
		case actFORWARD:
			brujula = static_cast<Orientacion> ((brujula)%8);
			break;
		case actIDLE:
			break; 
	
	}

	// Determinar la siguiente acción a realizar
	// Determinar la siguiente acción a realizar entre actTURN_BL, actTURN_BR, actTURN_SL, actTURN_SR, actFORWARD, actIDLE
	if (brujula == Norte){
		current_action = actTURN_BL; // Girar a la izquierda (sentido horario)
	}
	else if (brujula == Sur){
		current_action = actTURN_BR; // Girar a la derecha (sentido horario)
	}
	else if (brujula == Noroeste){
		current_action = actTURN_SL; // Girar suavemente a la izquierda
	}
	else if (brujula == Noreste){
		current_action = actTURN_SR; // Girar suavemente a la derecha
	}
	else if (brujula == Sureste || brujula == Suroeste){
		current_action = actFORWARD; // Avanzar en la dirección actual
	}
	else {
		current_action = actIDLE; // No hacer nada
	}

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
