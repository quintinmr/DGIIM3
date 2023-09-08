#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

int counterA = 2;
int counterB = 2;
int counterX = 0;
const int MAX_STEPS = 20;
int num_max_steps_forward = MAX_STEPS;

Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
	int a;
	
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
	switch(last_action){

	
		// Avanzar hacia adelante
		case actFORWARD:

			
			switch (current_state.brujula)
			{
				case norte:    current_state.fil--;break;
				case noreste:  current_state.fil--; current_state.col++; break;
				case este:	   current_state.col++; break;
				case sureste:  current_state.fil++; current_state.col++; break;
				case sur:	   current_state.fil++; break;
				case suroeste: current_state.fil++; current_state.col--; break;
				case oeste:	   current_state.col--; break;
				case noroeste: current_state.fil--; current_state.col--; break;
			}
				
			break;
		
		// Giro a la izquierda de 45º
		case actTURN_SL:
			a = current_state.brujula;
			a = (a+7)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			girar_derecha = (rand()%2==0);
			break;
		
		// Giro a la derecha de 45º
		case actTURN_SR:
			a = current_state.brujula;
			a = (a+1)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			girar_derecha = (rand()%2==0);
			break;

		// Giro a la izquierda de 135º
		case actTURN_BL:
			a = current_state.brujula;
			a = (a+5)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;

		// Giro a la derecha de 135º
		case actTURN_BR:
			a = current_state.brujula;
			a = (a+3)%8;
			current_state.brujula = static_cast<Orientacion>(a);
			break;
		
	}

	if (sensores.posF!=-1 and !bien_situado){
		current_state.fil = sensores.posF;
		current_state.col= sensores.posC;
		current_state.brujula = sensores.sentido;
		bien_situado = true;
	}

	if (bien_situado){
		
		PonerTerrenoEnMatriz(sensores.terreno, current_state, mapaResultado);
	}

	if (sensores.colision == true) sensores.reset = true;
	
	if (sensores.reset == true){
		// En caso de que se haya reseteado hemos de cambiar los parámetros del agente:
		current_state.brujula 				= norte;
		current_state.fil 					= current_state.col = 99;
		last_action							= actIDLE;
		bien_situado						= false;
		girar_derecha						= false;
		bikini_ON							= false;
		zapatillas_ON						= false;
		todo_agua							= false;
		todo_bosque							= false;
		vector<int> empty (2*MAX_SIZE,0);
      	for (int i = 0; i < 2*MAX_SIZE; i++) counter.push_back(empty);
		vector<unsigned char> empt_y (2*MAX_SIZE,'?');
      	for (int i = 0; i < 2*MAX_SIZE; i++) mapaResultado.push_back(empt_y);
	
	}

	// Decisión de la nueva acción
	else if ((sensores.terreno[0] == 'X') and sensores.bateria < 4000) accion = actIDLE;
	else if((sensores.terreno[1] == 'X' or sensores.terreno[4] == 'X' or sensores.terreno[9] == 'X') and
	        (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' )){
		accion = actTURN_SL;
	}
	else if((sensores.terreno[2] == 'X' or sensores.terreno[6] == 'X' or sensores.terreno[12] == 'X') and
	        (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )){
		if (counterX == 2){
			accion = DerechaoIzquierda(girar_derecha); 
			counterX = 0;
		}
		else{
			counterX++; 
			accion = actFORWARD;
		}
	}
	else if((sensores.terreno[3] == 'X' or sensores.terreno[8] == 'X' or sensores.terreno[15] == 'X') and
	        (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' )){
		accion = actTURN_SR;
	}
	else if((sensores.terreno[2] == 'K') and (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and (bikini_ON == false) and (sensores.superficie[2] == '_') ){
			
		accion = actFORWARD;
		bikini_ON = true;
			
	}
	else if (sensores.terreno[2] == 'K' and bikini_ON == true) accion = actTURN_BR;
	else if (sensores.terreno[1] == 'K' and sensores.terreno[3] == 'K' and (bikini_ON == false)){

		accion = DerechaoIzquierda(girar_derecha);	
	}
	else if((sensores.terreno[1] == 'K' or sensores.terreno[4] == 'K' or sensores.terreno[9] == 'K') and
	        (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' ) and ((sensores.terreno[3] != 'K' and sensores.terreno[8] != 'K' and sensores.terreno[15] != 'K')) and (bikini_ON == false) and (sensores.superficie[1] == '_')){
			
			if ((sensores.terreno[1] != 'A' and sensores.terreno[1] !=  'B')) accion = actTURN_SL;
			else accion = actFORWARD;
		
	}
	else if((sensores.terreno[3] == 'K' or sensores.terreno[8] == 'K' or sensores.terreno[15] == 'K') and
	        (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' ) and ((sensores.terreno[1] != 'K' and sensores.terreno[4] != 'K' and sensores.terreno[9] != 'K')) and (bikini_ON == false) and (sensores.superficie[3] == '_')){
			
			if ((sensores.terreno[3] != 'A' and sensores.terreno[3] !=  'B')) accion = actTURN_SR;
			else accion = actFORWARD;
		
	}
	else if (sensores.terreno[1] == 'D' and sensores.terreno[3] == 'D' and (zapatillas_ON == false)){

		accion = DerechaoIzquierda(girar_derecha);	
	}
	else if((sensores.terreno[1] == 'D' or sensores.terreno[4] == 'D' or sensores.terreno[9] == 'D') and
	        (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' ) and ((sensores.terreno[3] != 'D' and sensores.terreno[8] != 'D' and sensores.terreno[15] != 'D')) and (zapatillas_ON == false)  and (sensores.superficie[1] == '_') ){
			
			//accion = actTURN_SL;
			 if ((sensores.terreno[1] != 'A' and sensores.terreno[1] !=  'B')) accion = actTURN_SL;
			else accion = actFORWARD;
		
	}
	else if((sensores.terreno[2] == 'D') and (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and (zapatillas_ON == false) and (sensores.superficie[2] == '_') ){
			
		accion = actFORWARD;
		zapatillas_ON = true;
			
	}
	else if((sensores.terreno[3] == 'D' or sensores.terreno[8] == 'D' or sensores.terreno[15] == 'D') and
	        (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' ) and ((sensores.terreno[1] != 'D' and sensores.terreno[4] != 'D' and sensores.terreno[9] != 'D')) and (zapatillas_ON == false) and (sensores.superficie[3] == '_')){
			
			 if ((sensores.terreno[3] != 'A' and sensores.terreno[3] !=  'B')) accion = actTURN_SR;
			else accion = actFORWARD;
		
	}
	else if (sensores.terreno[2] == 'D' and zapatillas_ON == true) accion = actTURN_BR;
	else if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G' or ((sensores.terreno[2] == 'A'/*  or sensores.terreno[2] == 'K' */) and (bikini_ON == true)) or
			 ((sensores.terreno[2] == 'B'/*  or sensores.terreno[2] == 'D' */) and (zapatillas_ON == true))) and sensores.superficie[2]=='_'){
		
		if (num_max_steps_forward > 0){
			 accion = actFORWARD;
			 cout << num_max_steps_forward;
			 
		}
		else{
			accion = AccionMaximaPrioridad(current_state, sensores);
		}
	}
	else if (sensores.terreno[2]=='A' and sensores.superficie[2]=='_'){

		if (bikini_ON) accion =  actFORWARD;
		else if (!girar_derecha){
		
			accion = actTURN_SL;
			girar_derecha = (rand()%2==0);

		}
		else if (girar_derecha){

			accion = actTURN_SR;
			girar_derecha = (rand()%2==0);
			
		}
		
	}
	else if (sensores.terreno[2]=='B' and sensores.superficie[2]=='_'){

		if (zapatillas_ON) accion = actFORWARD;
		else if (!girar_derecha){
		
				accion = actTURN_SL;
				girar_derecha = (rand()%2==0);

		}
		else if (girar_derecha){
				accion = actTURN_SR;
				girar_derecha = (rand()%2==0);
			
		}
		
	}
	else if (sensores.sentido == norte or sensores.sentido == sur){

		if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
			 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			 (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			accion = actTURN_SR;
		}

		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
				(sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
				(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){
			accion = actTURN_SR;
		}

		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
				(sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
				(sensores.terreno[3] == 'P' and sensores.terreno[3] == 'M')){
			accion = actTURN_SL;
		}

	}
	else if (sensores.sentido == noreste){

		if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = actTURN_SR;
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			
			accion = actTURN_SL;
		
		}
		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
			 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			 (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			accion = actTURN_BR;
		}
		
	}
	else if (sensores.sentido == este or sensores.sentido == oeste){

		
		if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and  // e y o
			 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			 (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			accion = actTURN_BR;
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and  // e y o
				 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M')){

			accion = actTURN_SL;
		}
		else if ((sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M') and // e y o
				 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M')){

			accion = actTURN_SR;
		}
		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and  // e y o
				 (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = actTURN_SR;
		}
		else if ((sensores.terreno[1] == 'A' and sensores.terreno[3] == 'A') and  // e y o
			 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M')){
			accion = actTURN_BL;
		}
		else if ((sensores.terreno[1] == 'B' and sensores.terreno[3] == 'B') and  // e y o
			 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M')){
			accion = actTURN_BL;
		}
	}
	else if (sensores.sentido == noroeste){

		
		if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			
			accion = actTURN_BR;
		
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			
			accion = actTURN_SL;
		
		}
		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = actTURN_SR;
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = DerechaoIzquierda(girar_derecha);
		}
	}
	else if (sensores.sentido == sureste){

		
		if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2]  == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3]  == 'M')){
			
			accion = actTURN_BL;
		
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			
			accion = actTURN_SL;
		
		}
		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = actTURN_SR;
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = DerechaoIzquierda(girar_derecha);
		}
	}
	else if (sensores.sentido == suroeste){

		if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2]  == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3]  == 'M')){
			
			accion = actTURN_BR;
		
		}

		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')){

			accion = actTURN_SR;
		}
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and
		    (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			(sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			
			accion = actTURN_SL;
		
		}
		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and
			 (sensores.terreno[2] == 'P' or sensores.terreno[2] == 'M') and
			 (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
			accion = actTURN_BL;
		}
	}
	
	if (num_max_steps_forward == 0) num_max_steps_forward = MAX_STEPS;
	num_max_steps_forward--;
	last_action = accion;
	return accion;
}


void ComportamientoJugador::PonerTerrenoEnMatriz(vector<unsigned char> terreno, const state &st, vector<vector<unsigned char>> &matriz){

	// Dependiendo de la orientación del agente podremos pintar un número concreto de casillas
	// de las 16 que puede ver (que están almacenadas en sensores.terreno)
	matriz[st.fil][st.col] 		= terreno[0];
	counter[st.fil][st.col]++;

	switch (st.brujula)
	{
	case norte:
		
		matriz[st.fil-1][st.col] 	= terreno[2];
		matriz[st.fil-1][st.col-1] 	= terreno[1];
		matriz[st.fil-1][st.col+1] 	= terreno[3];
		matriz[st.fil-2][st.col-2] 	= terreno[4];
		matriz[st.fil-2][st.col-1] 	= terreno[5];
		matriz[st.fil-2][st.col] 	= terreno[6];
		matriz[st.fil-2][st.col+1] 	= terreno[7];
		matriz[st.fil-2][st.col+2] 	= terreno[8];
		matriz[st.fil-3][st.col-3] 	= terreno[9];
		matriz[st.fil-3][st.col-2] 	= terreno[10];
		matriz[st.fil-3][st.col-1] 	= terreno[11];
		matriz[st.fil-3][st.col] 	= terreno[12];
		matriz[st.fil-3][st.col+1] 	= terreno[13];
		matriz[st.fil-3][st.col+2] 	= terreno[14];
		matriz[st.fil-3][st.col+3] 	= terreno[15];
		
		break;
	
	case sur:

		matriz[st.fil][st.col] 		= terreno[0];
		matriz[st.fil+1][st.col] 	= terreno[2];
		matriz[st.fil+1][st.col-1] 	= terreno[3];
		matriz[st.fil+1][st.col+1] 	= terreno[1];
		matriz[st.fil+2][st.col-2] 	= terreno[8];
		matriz[st.fil+2][st.col-1] 	= terreno[7];
		matriz[st.fil+2][st.col] 	= terreno[6];
		matriz[st.fil+2][st.col+1] 	= terreno[5];
		matriz[st.fil+2][st.col+2] 	= terreno[4];
		matriz[st.fil+3][st.col-3] 	= terreno[15];
		matriz[st.fil+3][st.col-2] 	= terreno[14];
		matriz[st.fil+3][st.col-1] 	= terreno[13];
		matriz[st.fil+3][st.col] 	= terreno[12];
		matriz[st.fil+3][st.col+1] 	= terreno[11];
		matriz[st.fil+3][st.col+2] 	= terreno[10];
		matriz[st.fil+3][st.col+3] 	= terreno[9];
		
	break;

	case este:
		
		matriz[st.fil][st.col+1] 	= terreno[2];
		matriz[st.fil-1][st.col+1] 	= terreno[1];
		matriz[st.fil+1][st.col+1] 	= terreno[3];
		matriz[st.fil-2][st.col+2] 	= terreno[4];
		matriz[st.fil-1][st.col+2] 	= terreno[5];
		matriz[st.fil][st.col+2] 	= terreno[6];
		matriz[st.fil+1][st.col+2] 	= terreno[7];
		matriz[st.fil+2][st.col+2] 	= terreno[8];
		matriz[st.fil-3][st.col+3] 	= terreno[9];
		matriz[st.fil-2][st.col+3] 	= terreno[10];
		matriz[st.fil-1][st.col+3] 	= terreno[11];
		matriz[st.fil][st.col+3] 	= terreno[12];
		matriz[st.fil+1][st.col+3] 	= terreno[13];
		matriz[st.fil+2][st.col+3] 	= terreno[14];
		matriz[st.fil+3][st.col+3] 	= terreno[15];
		
	break;

	case oeste:

		matriz[st.fil][st.col-1] 	= terreno[2];
		matriz[st.fil-1][st.col-1] 	= terreno[3];
		matriz[st.fil+1][st.col-1] 	= terreno[1];
		matriz[st.fil-2][st.col-2] 	= terreno[8];
		matriz[st.fil-1][st.col-2] 	= terreno[7];
		matriz[st.fil][st.col-2] 	= terreno[6];
		matriz[st.fil+1][st.col-2] 	= terreno[5];
		matriz[st.fil+2][st.col-2] 	= terreno[4];
		matriz[st.fil-3][st.col-3] 	= terreno[15];
		matriz[st.fil-2][st.col-3] 	= terreno[14];
		matriz[st.fil-1][st.col-3] 	= terreno[13];
		matriz[st.fil][st.col-3] 	= terreno[12];
		matriz[st.fil+1][st.col-3] 	= terreno[11];
		matriz[st.fil+2][st.col-3] 	= terreno[10];
		matriz[st.fil+3][st.col-3] 	= terreno[9];
		
	break;

	case noreste:
		
		matriz[st.fil][st.col+1] 	= terreno[3];
		matriz[st.fil][st.col+2] 	= terreno[8];
		matriz[st.fil][st.col+3] 	= terreno[15];
		matriz[st.fil-1][st.col] 	= terreno[1];
		matriz[st.fil-1][st.col+1] 	= terreno[2];
		matriz[st.fil-1][st.col+2] 	= terreno[7];
		matriz[st.fil-1][st.col+3] 	= terreno[14];
		matriz[st.fil-2][st.col] 	= terreno[4];
		matriz[st.fil-2][st.col+1] 	= terreno[5];
		matriz[st.fil-2][st.col+2] 	= terreno[6];
		matriz[st.fil-2][st.col+3] 	= terreno[13];
		matriz[st.fil-3][st.col] 	= terreno[9];
		matriz[st.fil-3][st.col+1] 	= terreno[10];
		matriz[st.fil-3][st.col+2] 	= terreno[11];
		matriz[st.fil-3][st.col+3] 	= terreno[12];
		
	break;

	case noroeste:
		
		matriz[st.fil][st.col-1] 	= terreno[1];
		matriz[st.fil][st.col-2] 	= terreno[4];
		matriz[st.fil][st.col-3] 	= terreno[9];
		matriz[st.fil-1][st.col] 	= terreno[3];
		matriz[st.fil-1][st.col-1] 	= terreno[2];
		matriz[st.fil-1][st.col-2] 	= terreno[5];
		matriz[st.fil-1][st.col-3] 	= terreno[10];
		matriz[st.fil-2][st.col] 	= terreno[8];
		matriz[st.fil-2][st.col-1] 	= terreno[7];
		matriz[st.fil-2][st.col-2] 	= terreno[6];
		matriz[st.fil-2][st.col-3] 	= terreno[11];
		matriz[st.fil-3][st.col] 	= terreno[15];
		matriz[st.fil-3][st.col-1] 	= terreno[14];
		matriz[st.fil-3][st.col-2] 	= terreno[13];
		matriz[st.fil-3][st.col-3] 	= terreno[12];
		
	break;

	case sureste:

		matriz[st.fil][st.col+1] 	= terreno[1];
		matriz[st.fil][st.col+2] 	= terreno[4];
		matriz[st.fil][st.col+3] 	= terreno[9];
		matriz[st.fil+1][st.col] 	= terreno[3];
		matriz[st.fil+1][st.col+1] 	= terreno[2];
		matriz[st.fil+1][st.col+2] 	= terreno[5];
		matriz[st.fil+1][st.col+3] 	= terreno[10];
		matriz[st.fil+2][st.col] 	= terreno[8];
		matriz[st.fil+2][st.col+1] 	= terreno[7];
		matriz[st.fil+2][st.col+2] 	= terreno[6];
		matriz[st.fil+2][st.col+3] 	= terreno[11];
		matriz[st.fil+3][st.col] 	= terreno[15];
		matriz[st.fil+3][st.col+1] 	= terreno[14];
		matriz[st.fil+3][st.col+2] 	= terreno[13];
		matriz[st.fil+3][st.col+3] 	= terreno[12];
		
	break;

	case suroeste:
		
		matriz[st.fil][st.col-1] 	= terreno[3];
		matriz[st.fil][st.col-2] 	= terreno[8];
		matriz[st.fil][st.col-3] 	= terreno[15];
		matriz[st.fil+1][st.col] 	= terreno[1];
		matriz[st.fil+1][st.col-1] 	= terreno[2];
		matriz[st.fil+1][st.col-2] 	= terreno[7];
		matriz[st.fil+1][st.col-3] 	= terreno[14];
		matriz[st.fil+2][st.col] 	= terreno[4];
		matriz[st.fil+2][st.col-1] 	= terreno[5];
		matriz[st.fil+2][st.col-2] 	= terreno[6];
		matriz[st.fil+2][st.col-3] 	= terreno[13];
		matriz[st.fil+3][st.col] 	= terreno[9];
		matriz[st.fil+3][st.col-1] 	= terreno[10];
		matriz[st.fil+3][st.col-2] 	= terreno[11];
		matriz[st.fil+3][st.col-3] 	= terreno[12];
		
	break;
	}
}

Action ComportamientoJugador::AccionMaximaPrioridad(const state &st, Sensores sensors){
	
	int contadorPos1 = 0;
	int contadorPos2 = 0;
	int contadorPos3 = 0;
	int count        = 2;
	Action  action;

	switch (st.brujula)
	{
	case norte:

		contadorPos1 = counter[st.fil-1][st.col-1];
		contadorPos2 = counter[st.fil-1][st.col];
		contadorPos3 = counter[st.fil-1][st.col+1];

		break;

	case sur:

		contadorPos1 = counter[st.fil+1][st.col+1];
		contadorPos2 = counter[st.fil+1][st.col];
		contadorPos3 = counter[st.fil+1][st.col-1];

		
		break;

	case este:

		contadorPos1 = counter[st.fil-1][st.col+1];
		contadorPos2 = counter[st.fil][st.col+1];
		contadorPos3 = counter[st.fil+1][st.col+1];

		break;

	case oeste:

		contadorPos1 = counter[st.fil+1][st.col-1];
		contadorPos2 = counter[st.fil][st.col-1];
		contadorPos3 = counter[st.fil-1][st.col-1];

		break;

	case noreste:

		contadorPos1 = counter[st.fil-1][st.col];
		contadorPos2 = counter[st.fil-1][st.col+1];
		contadorPos3 = counter[st.fil][st.col+1];

		break;

	case noroeste:

		contadorPos1 = counter[st.fil][st.col-1];
		contadorPos2 = counter[st.fil-1][st.col-1];
		contadorPos3 = counter[st.fil-1][st.col];

		break;

	case sureste:

		contadorPos1 = counter[st.fil][st.col+1];
		contadorPos2 = counter[st.fil+1][st.col+1];
		contadorPos3 = counter[st.fil+1][st.col];

		break;

	case suroeste:

		contadorPos1 = counter[st.fil+1][st.col];
		contadorPos2 = counter[st.fil+1][st.col-1];
		contadorPos3 = counter[st.fil][st.col-1];

		break;

	}

	if (contadorPos1 == contadorPos2 == contadorPos3){
			
			
			switch (count)
			{
			case 0:
				action = actTURN_SL;
				count = 2;
				break;

			case 1:
				action = actTURN_SR;
				count--;
				break;
			
			case 2:
				action = actFORWARD;
				count--;
				break;
			
			}
	}

	if ((contadorPos1<contadorPos2) and sensors.terreno[1] != 'P' and sensors.terreno[1] != 'M'){

		action = actTURN_SL;


		if ((contadorPos3 < contadorPos1) and sensors.terreno[3] != 'P' and sensors.terreno[3] != 'M') action = actTURN_SR;

	}
	else if ((contadorPos3 < contadorPos2) and sensors.terreno[3] != 'P' and sensors.terreno[3] != 'M') action = actTURN_SR;

	return action;

}

int ComportamientoJugador::interact(Action accion, int valor){
    return false;
}

Action ComportamientoJugador::DerechaoIzquierda(bool &giro){

	Action act;

	if(!giro){

		act = actTURN_SL;
		giro = (rand()%2 == 0);

	}else{
		act = actTURN_SR;
		giro = (rand()%2 == 0);
	} 

	return act;			
}