#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

const int MAX_STEPS = 50;
int counterA 		= 2;
int counterB 		= 2;
int count 		= MAX_STEPS;
int max_priority = 2;


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
			a = (a-3)%8;
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

	// Decisión de la nueva acción

	//------------------------------------------------

	// Si el agente percibe a través de los sensores una colisión, se resetea el juego
	
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
		counter.resize(rows, vector<int>(cols, 0));
		mapaResultado.resize(rows,vector<unsigned char>(cols, '?'));

	
	}
	// Si paso por la casilla de recarga, dependerá el permanecer en ella de los ciclos de vida que me 
	// queden. Si me quedan pocos ciclos de vida, simplemente paso una sola vez por ella y me olvido de ella.
	// Si por el contrario me quedan todavía ciclos de vida suficientes, merece la pena recargar batería permaneciendo
	// en ella hasta que se recargue la batería.
	else if (sensores.terreno[0] == 'X' and sensores.superficie[0] == '_'){
		if (sensores.vida > 1000 or sensores.bateria < 5000) accion = actFORWARD;
		else accion = actIDLE;
	}
	else if((sensores.terreno[1] == 'X' or sensores.terreno[4] == 'X' or sensores.terreno[9] == 'X') and
	        (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' )and sensores.superficie[1] == '_' ){
		accion = actTURN_SL;
	}
	else if((sensores.terreno[2] == 'X' or sensores.terreno[6] == 'X' or sensores.terreno[12] == 'X') and
	        (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and sensores.superficie[2] == '_'){
		accion = actFORWARD;
	}
	else if((sensores.terreno[3] == 'X' or sensores.terreno[8] == 'X' or sensores.terreno[15] == 'X') and
	        (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' )and sensores.superficie[3] == '_' ){
		accion = actTURN_SR;
	}
	else if (sensores.terreno[1] == 'K' and sensores.terreno[3] == 'K' and (bikini_ON == false)){

		accion = DerechaoIzquierda(girar_derecha);	
	}
	else if((sensores.terreno[1] == 'K' or sensores.terreno[4] == 'K' or sensores.terreno[9] == 'K') and
	        (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' ) and (bikini_ON == false) and (sensores.superficie[1] == '_')){
			
		accion = actTURN_SL;
	
	}
	else if((sensores.terreno[2] == 'K') and (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and (bikini_ON == false) and (sensores.superficie[2] == '_') ){
			
		accion = actFORWARD;
			
	}
	else if((sensores.terreno[2] == 'K') and (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and (bikini_ON == true) and (sensores.superficie[2] == '_') ){
			
		accion = actTURN_BL;
			
	}
	else if((sensores.terreno[3] == 'K' or sensores.terreno[8] == 'K' or sensores.terreno[15] == 'K') and (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' ) and (bikini_ON == false) and (sensores.superficie[3] == '_') ){
			
		accion    = actTURN_SR;

	}
	else if (sensores.terreno[1] == 'D' and sensores.terreno[3] == 'D' and (zapatillas_ON == false)){

		accion = DerechaoIzquierda(girar_derecha);	
	}
	else if((sensores.terreno[1] == 'D' or sensores.terreno[4] == 'D' or sensores.terreno[9] == 'D') and
	        (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' ) and (zapatillas_ON == false) and (sensores.superficie[1] == '_')){
			
		accion = actTURN_SL;
	
	}
	else if((sensores.terreno[2] == 'D') and
	        (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and (zapatillas_ON == false) and (sensores.superficie[2] == '_') ){
			
		accion = actFORWARD;
			
	}
	else if((sensores.terreno[2] == 'D') and
	        (sensores.terreno[2] != 'P' and sensores.terreno[2] != 'M' )and (zapatillas_ON == true) and (sensores.superficie[2] == '_') ){
			
		accion = actTURN_BR;
			
	}
	else if((sensores.terreno[3] == 'D' or sensores.terreno[8] == 'D' or sensores.terreno[15] == 'D') and
	        (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' ) and (zapatillas_ON == false) and (sensores.superficie[3] == '_') ){
			
			accion    = actTURN_SR;

	}
	else if ((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G' /* or
	          sensores.terreno[2] == 'X' */) and sensores.superficie[2]=='_'){
			
			/*if (counter[current_state.fil][current_state.col] <= 2)
				accion = actFORWARD;
			else accion = AccionMaximaPrioridad(counter, current_state, sensores, contador, max_priority, girar_derecha);
			*/
		//accion = actFORWARD;
			if (count > 0){ 
				accion = actFORWARD;
				count--;
			}
			else{

				accion = AccionMaximaPrioridad(counter, current_state, sensores, count, max_priority, girar_derecha);
				count = 20;
			}
		
	}

	// Si lo que tengo delante es agua, la acción a realizar dependerá de si llevo bikini puesto, evidentemente
	// para poder avanzar tranquilamente por el agua, pero si no lo llevo, mi acción dependerá de mi entorno;
	// de las casillas que me rodean. Avanzaré a aquella casilla con la que gaste menos batería.
	else if (sensores.terreno[2] == 'A' and (bikini_ON == true) and sensores.superficie[2] == '_') accion = actFORWARD;	
	else if (sensores.terreno[2] == 'A' and (bikini_ON == false) and sensores.superficie[2] == '_' and todo_agua == false){	


		 if((sensores.terreno[0] == 'A') and /* (sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') */  (sensores.terreno[3] == 'B')) accion=actTURN_SR;

		else if ((sensores.terreno[1] == 'A' or sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and (sensores.terreno[3] == 'A' or sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')){
	
			switch (counterA)
			{
			case 2: case 1:
				accion = actTURN_BL;
				counterA--;
				break;
			
			case 0:
				accion = actFORWARD;
				counterA = 2;
				todo_agua = true;
				break;
			}
	
		}
		
		else if( sensores.terreno[0] == 'B'  and (sensores.terreno[1] == 'A' or sensores.terreno[3] == 'A')) accion = actTURN_BR;
		else if( sensores.terreno[0] == 'B'  and (sensores.terreno[1] == 'B' or sensores.terreno[3] == 'B')) accion = actTURN_SR;
		else if( sensores.terreno[0] == 'B'  and (sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M')  and ( sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')) accion=actTURN_BL;	
		else if((sensores.terreno[0] != 'A') and (sensores.terreno[1] !='A' and sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') ) accion=actTURN_SL;
		else if((sensores.terreno[0] != 'A') and (sensores.terreno[3] !='A' and sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M') ) accion=actTURN_SR;
		else if((sensores.terreno[0] != 'A' and sensores.terreno[0] != 'B' ) and ( sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')) accion=actTURN_BL;
		else if((sensores.terreno[0] != 'A' and sensores.terreno[0] != 'B' ) and(sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') ) accion=actTURN_BR;

		else if((sensores.terreno[0] == 'A') and (sensores.terreno[1] !='A' and sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' and (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M' )))  accion=actFORWARD;
		else if((sensores.terreno[0] == 'A') and (sensores.terreno[1] !='A' and sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M')) accion = actTURN_BL;
		else if((sensores.terreno[0] == 'A') and (sensores.terreno[3] !='A' and sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')) accion = actTURN_BR;
		else if( sensores.terreno[1] !='A' and sensores.terreno[3] !='A') accion=actTURN_SR;
		else if((sensores.terreno[0] == 'A') and (sensores.terreno[3] =='A') and (sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') ) accion=actTURN_BL;			
		else if((sensores.terreno[0] == 'A') and (sensores.terreno[1] =='A') and (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M') ) accion=actTURN_BR;
		

	}
	else if (sensores.terreno[2] == 'A' and !bikini_ON and sensores.superficie[2] == '_' and todo_agua == true){	

		if (sensores.terreno[1] == 'A' and sensores.terreno[3] == 'A') accion = actFORWARD;
		else if (sensores.terreno[1] == 'A' and (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' and sensores.terreno[3] != 'A')){accion = actTURN_BR; todo_agua = false;}
		else if (sensores.terreno[1] == 'A' and (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M' or sensores.terreno[3] == 'A')){accion = actFORWARD; todo_agua = false;}
		else if (sensores.terreno[3] == 'A' and (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' and sensores.terreno[1] != 'A')){accion = actTURN_SL; todo_agua = false;}
		else if (sensores.terreno[3] == 'A' and (sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M' or sensores.terreno[1] == 'A')){accion = actFORWARD; todo_agua = false;}

	}

	else if (sensores.terreno[2] == 'B' and (zapatillas_ON == true) and sensores.superficie[2] == '_') accion = actFORWARD;	
	else if (sensores.terreno[2] == 'B' and (zapatillas_ON == false) and sensores.superficie[2] == '_' and todo_bosque == false){	

		
		if ((sensores.terreno[1] == 'B' or sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M') and sensores.terreno[3] == 'B'){

			
			switch (counterB)
			{
			case 2: case 1:
				accion = actTURN_BL;
				counterB--;
				break;
			
			case 0:
				accion = actFORWARD;
				counterB = 2;
				todo_bosque = true;
				break;
			}
			
		}

		else if (sensores.terreno[0] == 'A') accion = actFORWARD;
		else if (sensores.terreno[1] == 'B' and (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')) accion = actFORWARD;
		else if (sensores.terreno[3] == 'B' and (sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M')) accion = actFORWARD;
		else if (sensores.terreno[1] == 'B' and (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' and sensores.terreno[3] == 'A')) accion = actTURN_SR;
		else if (sensores.terreno[3] == 'B' and (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' and sensores.terreno[1] == 'A')) accion = actTURN_SL;
		else if ((sensores.terreno[1] == 'M' or sensores.terreno[1] == 'P'))accion = actFORWARD;
		else if ((sensores.terreno[3] == 'M' or sensores.terreno[3] == 'P')) accion = actFORWARD;
		else if (sensores.terreno[1] == 'B' and sensores.terreno[3] == 'A') accion = actTURN_BL;
		else if (sensores.terreno[3] == 'B' and sensores.terreno[1] == 'A') accion = actTURN_BR;
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' and sensores.terreno[1] != 'A') and sensores.terreno[1]!='B') accion = actTURN_SL;
		else if ((sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' and sensores.terreno[3] != 'A') and sensores.terreno[3]!='B') accion = actTURN_SR;
		else if ((sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M') and (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M')) accion = actTURN_SR;
		else if ((sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M') and (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M')) accion = actTURN_SL;

	}
	else if (sensores.terreno[2] == 'B' and !zapatillas_ON and sensores.superficie[2] == '_' and todo_bosque == true){ //¿Qué ocurre si estoy todo rodeado de agua?

		if (sensores.terreno[1] == 'B' and sensores.terreno[3] == 'B') accion = actFORWARD;
		else if (sensores.terreno[1] == 'B' and (sensores.terreno[3] != 'P' and sensores.terreno[3] != 'M' and sensores.terreno[3] != 'B')){accion = actTURN_SR; todo_bosque = false;}
		else if (sensores.terreno[1] == 'B' and (sensores.terreno[3] == 'P' or sensores.terreno[3] == 'M' or sensores.terreno[3] == 'A')){accion = actTURN_SL; todo_bosque = false;}
		else if (sensores.terreno[3] == 'B' and (sensores.terreno[1] != 'P' and sensores.terreno[1] != 'M' and sensores.terreno[1] != 'B')){accion = actTURN_SL; todo_bosque = false;}
		else if (sensores.terreno[3] == 'B' and (sensores.terreno[1] == 'P' or sensores.terreno[1] == 'M' or sensores.terreno[1] == 'B')){accion = actTURN_SR; todo_bosque = false;}
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
		

	last_action = accion;
	return accion;
}


void ComportamientoJugador::PonerTerrenoEnMatriz(vector<unsigned char> terreno, const state &st, vector<vector<unsigned char>> &matriz){

	// Dependiendo de la orientación del agente podremos pintar un número concreto de casillas
	// de las 16 que puede ver (que están almacenadas en sensores.terreno)
	matriz[st.fil][st.col] = terreno[0];
	// Llevamos también un registro del número de veces que pasamos por una determinada casilla.
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

// La casilla más prioritaria en cada ciclo, será aquella por la que menos veces hayamos pasado.
// Es por ello que el contador que tenemos en el método anteior nos va a servir para discernir,
// en función de la orientación del agente, a qué casilla avanzar, con el objetivo de ahorrar 
// batería, evitando pasar muchas veces por casillas ya visitadas.

 Action ComportamientoJugador::AccionMaximaPrioridad(vector<vector<int>> cont, const state &st, Sensores sensors, 
                             int &count, int &max_prior, bool &giro){

	max_prior = 2; // estableceremos la casilla en la posición 2 como la más prioritaria
	int contadorPos1;
	int contadorPos2;
	int contadorPos3;
	Action a;

	switch (st.brujula){

		case norte:

			 contadorPos1 = cont[st.fil-1][st.col-1];
			 contadorPos2 = cont[st.fil-1][st.col];
			 contadorPos3 = cont[st.fil-1][st.col+1];
			
			break;

		case sur:

			 contadorPos1 = cont[st.fil+1][st.col+1];
			 contadorPos2 = cont[st.fil+1][st.col];
			 contadorPos3 = cont[st.fil+1][st.col-1];

			break;

		case este:

			 contadorPos1 = cont[st.fil-1][st.col+1];
			 contadorPos2 = cont[st.fil][st.col+1];
			 contadorPos3 = cont[st.fil+1][st.col+1];

			break;

		case oeste:

			 contadorPos1 = cont[st.fil+1][st.col-1];
			 contadorPos2 = cont[st.fil][st.col-1];
			 contadorPos3 = cont[st.fil-1][st.col-1];

		break;

		case noreste:

			 contadorPos1 = cont[st.fil-1][st.col];
			 contadorPos2 = cont[st.fil-1][st.col+1];
			 contadorPos3 = cont[st.fil][st.col+1];

		break;

		case sureste:

			 contadorPos1 = cont[st.fil][st.col+1];
			 contadorPos2 = cont[st.fil+1][st.col+1];
			 contadorPos3 = cont[st.fil+1][st.col+1];

			
		break;

		case noroeste:
		
			 contadorPos1 = cont[st.fil][st.col-1];
			 contadorPos2 = cont[st.fil-1][st.col-1];
			 contadorPos3 = cont[st.fil-1][st.col];

		break;

		case suroeste:

			 contadorPos1 = cont[st.fil][st.col+1];
			 contadorPos2 = cont[st.fil+1][st.col+1];
			 contadorPos3 = cont[st.fil+1][st.col];


		break;


	}

	if (contadorPos1 == contadorPos2 == contadorPos3){

			switch (count)
			{
			case 2:
				count--;
				return actFORWARD;
				break;
			
			case 1:
				count--;
				return actTURN_SL;
				break;
			
			case 0:
				count = 2;
				return actTURN_SR;
			}
	}
		
	if ((contadorPos1 < contadorPos2) and sensors.terreno[1] != 'P' and sensors.terreno[1] != 'M'){

		if ((contadorPos3 < contadorPos1) and sensors.terreno[3] != 'P' and sensors.terreno[3] != 'M'){

			max_prior = 3;
			return actTURN_SR;

	
		}

		else{
			
			max_prior = 1;
			return actTURN_SL;

		}

		
	}
	if ((contadorPos3 < contadorPos2) and sensors.terreno[3] != 'P' and sensors.terreno[3] != 'M'){
		max_prior = 3;
		return actTURN_SR;
	}

							 
}


	


int ComportamientoJugador::interact(Action accion, int valor){
    return false;
}

Action ComportamientoJugador::EnBuscaDelBikini (Sensores sensor){


	if (sensor.terreno[1] == 'K') return actTURN_SL;
	else if (sensor.terreno[2] == 'K') return actFORWARD;
	else if (sensor.terreno[3] == 'K') return actTURN_SR;
	else if (sensor.terreno[6] == 'K') return actFORWARD;
	else if (sensor.terreno[4] == 'K' or sensor.terreno[5] == 'K') return actTURN_SL;
	else if (sensor.terreno[7] == 'K' or sensor.terreno[8] == 'K') return actTURN_SR;
	else if (sensor.terreno[12] == 'K') return actFORWARD;
	else if (sensor.terreno[9] == 'K' or sensor.terreno[10] == 'K' or sensor.terreno[11] == 'K') return actTURN_SL;
	else if (sensor.terreno[13] == 'K' or sensor.terreno[14] == 'K' or sensor.terreno[15] == 'K') return actTURN_SR;
	
	return actTURN_BR;
}

Action ComportamientoJugador::EnBuscaDeLasZpatillas (Sensores sensor){

	if (sensor.terreno[1] == 'D') return actTURN_SL;
	else if (sensor.terreno[2] == 'D') return actFORWARD;
	else if (sensor.terreno[3] == 'D') return actTURN_SR;
	else if (sensor.terreno[6] == 'D') return actFORWARD;
	else if (sensor.terreno[4] == 'D' or sensor.terreno[5] == 'D') return actTURN_SL;
	else if (sensor.terreno[7] == 'D' or sensor.terreno[8] == 'D') return actTURN_SR;
	else if (sensor.terreno[12] == 'D') return actFORWARD;
	else if (sensor.terreno[9] == 'D' or sensor.terreno[10] == 'D' or sensor.terreno[11] == 'D') return actTURN_SL;
	else if (sensor.terreno[13] == 'D' or sensor.terreno[14] == 'D' or sensor.terreno[15] == 'D') return actTURN_SR;
	
	return actTURN_BR;

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







