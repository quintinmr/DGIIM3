#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <cmath>
#include <stdlib.h>

using namespace std;

string obtener_date_actual()
{
    auto start = std::chrono::system_clock::now(); 
    std::time_t date_hour = std::chrono::system_clock::to_time_t(start); 
    string cad = ctime(&date_hour);
    return cad;
}



string crear_saldo(){

    srand(time(NULL));
    float num_aleatorio=1.4 + rand() / static_cast<float>(RAND_MAX);
    
    string S = to_string(num_aleatorio).substr(0,5);
    return S;
}

int main (int argc, char* argv[])
{

    const float PRECIO_BILLETE_JOVEN = 0.61,
                PRECIO_BILLETE__DISCAPACITADOS = 0.57,
                PRECIO_BILLETE_PENSIONISTA = 0.00,
                PRECIO_BILLETE_ORDINARIO = 1.40,
                PRECIO_BILLETE_CREDIBUS_5 = 0.87,
                PRECIO_BILLETE_CREDIBUS_10 = 0.85,
                PRECIO_BILLETE_CREDIBUS_20 = 0.83;
    
    int CANT_REC;
    srand(time(NULL));
    string lineas[29] = {"4","5","8","9","11","21","33","N1","N3","N5","N6","N8","N9","25","C5","C30","C31","C32","C33","C34","C35","U1","U2","U3","S0","S2","111","121","13"};
    string clases[6] = {"Joven", "Universitario", "Ordinario", "PersonasConDiscapacidad", "Credibus", "Pensionista"};
    string credibus_precios[3] = {"0.87", "0.85", "0.83"};
    string CLASE = clases[0+rand()%(6)], 
           NOMBRE , 
           DATE = obtener_date_actual(), 
           LINEA = lineas[0+rand()%(28)], 
           ID = to_string(10000000+rand()%(50000000-10000000)),
           credibus_precio= credibus_precios[0+rand()%(2)];

    string SALDO_A = crear_saldo(); 
    float  PRECIO[7] = {PRECIO_BILLETE_JOVEN,PRECIO_BILLETE__DISCAPACITADOS,PRECIO_BILLETE_PENSIONISTA, PRECIO_BILLETE_ORDINARIO, PRECIO_BILLETE_CREDIBUS_5, PRECIO_BILLETE_CREDIBUS_10, PRECIO_BILLETE_CREDIBUS_20};

    //void ticar();
    //void recargar(int cantidad);
    //void crear_bonobus();
    string obtener_date_actual();
    //void solicitud_recarga(int decision);
    //void actualizar_bonobus(FILE *file);
    string crear_nombres();
    string crear_saldo();

    ifstream nombres;
    nombres.open(argv[1]);

    string name;

    srand(time(NULL));
    int num_aleatorio=1 + rand() % (101-1);
    int contador=1;

    while(getline(nombres, name)) {

        if(num_aleatorio == contador) {
            NOMBRE = name;

        }

        contador++;
    
    }

    cout << ID << endl;

    if(CLASE == "Pensionista"){
        cout << "0.000" << endl;
    }else{
        cout << SALDO_A << endl;
    }
    
    if(CLASE == "Joven" || CLASE == "Universitario"){
        cout << PRECIO_BILLETE_JOVEN << endl;
    }else{
          if(CLASE == "Ordinario"){
            cout << PRECIO_BILLETE_ORDINARIO << endl;
          }else{
              if(CLASE == "Credibus"){
                  cout << credibus_precio << endl;
              }else{
                  if(CLASE == "PersonasConDiscapacidad"){
                      cout << PRECIO_BILLETE__DISCAPACITADOS << endl;
                  }else{
                      if(CLASE == "Pensionista"){
                          cout << "0.00" << endl;
                      }
                  }
              }
            }
        }
    cout << CLASE << endl;

    if (CLASE != "Ordinario" && CLASE != "Credibus" )
    {
          cout << NOMBRE << endl;

    }
    cout << LINEA << endl;
    cout << DATE << endl;


    /*
    cout << "_______________________________________________" << endl;
    cout << endl;
    cout << "                     LINEA "                     << LINEA << "       " << endl;
    cout << "_______________________________________________" << endl;
    cout << endl;
    cout << "FECHA: " << DATE << endl;
    cout << "ID: " << ID << endl;
    cout << "Bonobus de clase: " << CLASE << endl;
    if (CLASE != "Ordinario" || CLASE != "Credibus" )
    {
          cout << "Nombre Completo: " << NOMBRE << endl;

    }
    cout << "Precio del billete: " << PRECIO_BILLETE_JOVEN << "€" << endl;
    cout << "Saldo actual: " << SALDO_A << endl;*/
   
    return 0;

}