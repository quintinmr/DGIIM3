#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

struct stateN0{
    ubicacion jugador;
    ubicacion sonambulo;
    bool zapatillas_ON;
    bool zapatillasSON_ON;
	  bool bikini_ON; 
    bool bikiniSON_ON;

    bool operator==(const stateN0 &st) const{
        return (jugador.f == st.jugador.f && jugador.c == st.jugador.c &&
        sonambulo.f == st.sonambulo.f && sonambulo.c == st.sonambulo.c &&
        jugador.brujula == st.jugador.brujula && sonambulo.brujula == st.sonambulo.brujula);
    }

    bool operator<(const stateN0 &st) const
    {
        return (jugador.f < st.jugador.f ||
               (jugador.f == st.jugador.f && jugador.c < st.jugador.c) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula < st.jugador.brujula) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f < st.sonambulo.f) || 
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f == st.sonambulo.f && sonambulo.c < st.sonambulo.c) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f == st.sonambulo.f && sonambulo.c == st.sonambulo.c && sonambulo.brujula < st.sonambulo.brujula) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f == st.sonambulo.f && sonambulo.c == st.sonambulo.c && sonambulo.brujula == st.sonambulo.brujula && bikini_ON < st.bikini_ON) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f == st.sonambulo.f && sonambulo.c == st.sonambulo.c && sonambulo.brujula == st.sonambulo.brujula && bikini_ON == st.bikini_ON && zapatillas_ON < st.zapatillas_ON) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f == st.sonambulo.f && sonambulo.c == st.sonambulo.c && sonambulo.brujula == st.sonambulo.brujula && bikini_ON == st.bikini_ON && zapatillas_ON == st.zapatillas_ON && bikiniSON_ON < st.bikiniSON_ON) ||
               (jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && sonambulo.f == st.sonambulo.f && sonambulo.c == st.sonambulo.c && sonambulo.brujula == st.sonambulo.brujula && bikini_ON == st.bikini_ON && zapatillas_ON == st.zapatillas_ON && bikiniSON_ON == st.bikiniSON_ON && zapatillasSON_ON < st.zapatillasSON_ON));

    }
};


struct nodeN0{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0 &nd) const{
    return (st == nd.st);
  }

  bool operator<(const nodeN0 &nd) const
  {
    return (st < nd.st);
  }
};

struct nodeN2{

  stateN0 st;
  int coste;
  list<Action> secuencia;

  bool operator==(const nodeN2 &nd) const{
    return (st == nd.st);
  }

  bool operator<(const nodeN2 &nd) const
  {
    return (st < nd.st);
  }

};

struct statecompare{

  bool operator()(const stateN0 &st1, const stateN0 &st2){

      return (st1 < st2);

  }
};

struct nodeN2compare{

  bool operator() (const nodeN2 &nd1, const nodeN2 &nd2){

    bool menor = false;
    if (nd2.coste < nd1.coste) return true;
    else if (nd1.coste == nd2.coste){
        statecompare()(nd1.st,nd2.st);
    }

    return menor;
    
  }
};


struct nodeN3{

  stateN0 st;
  int coste;
  double heuristica;
  double costPlusHeu;
  list<Action> secuencia;

  bool operator==(const nodeN3 &nd) const{
    return (st == nd.st);
  }

  bool operator<(const nodeN3 &nd) const
  {
    return (costPlusHeu < nd.costPlusHeu);
  }

};

struct heuristicacompare{
	bool operator()(const nodeN3 &nd1, const nodeN3 &nd2) const{

		bool a = false;

		if (nd2.costPlusHeu < nd1.costPlusHeu) a = true;
		else{
			if (nd1.costPlusHeu == nd2.costPlusHeu)
				statecompare()(nd1.st, nd2.st);
			else
				return false;
		}
		return a;
	}
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
      c_state.jugador.f = c_state.jugador.c = 99;
      c_state.sonambulo.f = c_state.sonambulo.c = 0;
      c_state.jugador.brujula = c_state.sonambulo.brujula = goal.brujula = norte;
      goal.f = goal.c = 99;

    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    bool AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa);
    bool AnchuraSoloJugadorN1(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa);
    bool DijkstraJugador(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa);
    bool AEstrellaSonambulo(const stateN0 &inicio, const ubicacion &final, list<Action> & plan, const vector<vector<unsigned char>> &mapa);
    bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa); 
    ubicacion NextCasilla(const ubicacion &pos);
    stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa);
    bool Find(const stateN0 &item, const list<nodeN0> &lista);
    void AnularMatriz(vector<vector<unsigned char>> &matriz);
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    bool VeoSonambulo(stateN0 &jug);
    int distanciaDelMaximo(const ubicacion &son, const ubicacion &dest);
    int costeDeBateria(unsigned char surface, bool bikini_ON, bool zapatillas_ON, Action accion);
    void actualizarNodo(stateN0 &st);
    bool veoSonambulo(const ubicacion &jg, const ubicacion &sn);
  private:
    // Declarar Variables de Estado

    list<Action> plan;  // Almacena el plan en ejecución
    bool hayPlan;       // Si verdad indica que se está ejecutando un plan
    stateN0 c_state;
    ubicacion goal;



};

#endif
