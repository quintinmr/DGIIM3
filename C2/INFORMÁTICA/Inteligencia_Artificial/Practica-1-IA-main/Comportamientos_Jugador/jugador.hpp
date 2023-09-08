#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{

  int fil;
  int col;
  Orientacion brujula;
  
};

const int MAX_SIZE = 100;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      last_action           = actIDLE;
      current_state.fil     = current_state.col=99;
      current_state.brujula = norte;
      girar_derecha         = false;
      bien_situado          = false;
      bikini_ON             = false;
      zapatillas_ON         = false;
      todo_agua = todo_bosque = false;
      vector<int> empty (2*MAX_SIZE,0);
      for (int i = 0; i < 2*MAX_SIZE; i++) counter.push_back(empty);
      
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void PonerTerrenoEnMatriz(vector<unsigned char> terreno, const state &st,
                              vector<vector<unsigned char>> &matriz);
    Action EnBuscaDelBikini (Sensores sensor);
    Action EnBuscaDeLasZpatillas (Sensores sensor);
    Action DerechaoIzquierda (bool &giro);
    Action AccionMaximaPrioridad(const state &st,
                               Sensores sensors);

  private:
  
  // Declarar aquí las variables de estado
  state current_state;
  Action last_action;
  bool girar_derecha;
  bool bien_situado;
  bool bikini_ON;
  bool zapatillas_ON;
  int rows = mapaResultado.size();
  int cols = mapaResultado.size();
  vector<vector<int>> counter;
  bool todo_agua;
  bool todo_bosque;

};

#endif