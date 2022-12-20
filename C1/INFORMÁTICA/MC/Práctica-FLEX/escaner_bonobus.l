%option noyywrap

/* @author: Noura Lachhab y Quintín Mesa Romero */
/* @grado: DGIIM                                */
/* @file: escaner_bonobus.l                     */

/*Sección de declaraciones*/

%{

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime>
#include <cmath>
#include <stdlib.h>

using namespace std;

ifstream bonobus;
const float PRECIO_BILLETE_JOVEN = 0.61,
          PRECIO_BILLETE__DISCAPACITADOS = 0.57,
          PRECIO_BILLETE_PENSIONISTA = 0,
          PRECIO_BILLETE_ORDINARIO = 1.40,
          PRECIO_BILLETE_CREDIBUS_5 = 0.87,
          PRECIO_BILLETE_CREDIBUS_10 = 0.85,
          PRECIO_BILLETE_CREDIBUS_20 = 0.83;

int CANT_REC;
string lineas[29] = {"4","5","8","9","11","21","33","N1","N3","N5","N6","N8","N9","25","C5","C30","C31","C32","C33","C34","C35","U1","U2","U3","S0","S2","111","121","13"};
string CLASE, NOMBRE, DATE, LINEA, ID;
float SALDO_A, PRECIO;

void ticar();
void recargar(int cantidad);
void crear_bonobus();
string obtener_date_actual();
void solicitud_recarga(int decision);
void actualizar_bonobus(FILE *file);

%}

DIGITO          [0-9]
MAY             [A-Z]
MIN             [a-z]  
DECIMAL         {DIGITO}+"."{DIGITO}{DIGITO}
SALDODECIMAL    {DIGITO}+"."{DIGITO}{DIGITO}{DIGITO}
ESPACIO         [[:blank:]]   
ALPHA           [A-Za-z]  
NAME            ({ALPHA}*{ESPACIO}{ALPHA}*)
FECHA           ({ALPHA}*{ESPACIO}{ALPHA}*{ESPACIO}{DIGITO}*{ESPACIO}{DIGITO}*"\:"{DIGITO}*"\:"{DIGITO}*{ESPACIO}{DIGITO}*)
LINE            ({MAY}{DIGITO}*|{DIGITO}|{DIGITO}{DIGITO}{DIGITO})
IDI             ({DIGITO}{DIGITO}{DIGITO}{DIGITO}{DIGITO}{DIGITO}{DIGITO}{DIGITO})
end             [ \n\t]


%%


    /*SECCIÓN DE REGLAS*/

{IDI}                                                                           {ID= yytext;}
{SALDODECIMAL}                                                                  {SALDO_A = stof(yytext);}
{DECIMAL}                                                                       {PRECIO = stof(yytext);}
(Joven|Universitario|Ordinario|PersonasConDiscapacidad|Credibus|Pensionista)    {CLASE = yytext;}
{NAME}                                                                          {NOMBRE= yytext;}
{LINE}                                                                          {LINEA = yytext;}
{FECHA}                                                                         {DATE= yytext;}

%%

int main (int argc, char* argv[])
{

    int option;
    cout << "HAS MONTADO EN EL AUTOBÚS" << endl;
    cout << "Tienes dos opciones: " <<endl;
    cout << " TICAR(1)  |  PEDIR CREDIBUS AL CONDUCTOR (2):" << endl;
    cin >> option;

    switch (option)
    {
        case 1:
        {   
            if (argc == 2)
            {
                bonobus.open(argv[1]);
                if (!bonobus.good()){
                    cout << "NO HAS PUESTO BIEN EL BONOBUS EN LA MÁQUINA"<< endl;
                    exit(1);
                }
        
            }
            else exit(1);

            yyFlexLexer flujo (&bonobus, 0);
            flujo.yylex();
            ticar();
            FILE *pfile = fopen(argv[1], "wt");
            actualizar_bonobus(pfile);
        }
        break;

        case 2:
        {
            crear_bonobus();
            solicitud_recarga(1);
            FILE *pfile = fopen(argv[1], "wt");
            actualizar_bonobus(pfile);
        }
        break;
    }
    
   
    return 0;

}

void ticar()
{
    cout << "_______________________________________________" << endl;
    cout << endl;
    cout << "                     LINEA "                     << LINEA << "       " << endl;
    cout << "_______________________________________________" << endl;
    cout << endl;
    cout << "FECHA: " << DATE << endl;
    cout << "ID: " << ID << endl;
    cout << "Bonobus de clase: " << CLASE << endl;

    if (CLASE == "Joven")
    {
        cout << "Nombre Completo: " << NOMBRE << endl;
        cout << "Precio del billete: ";
        cout << PRECIO_BILLETE_JOVEN << "€" << endl;
        if (SALDO_A < PRECIO_BILLETE_JOVEN ){
            cout << "Saldo actual: " << SALDO_A << endl;
            solicitud_recarga(0);
        }
        else 
        {   
            SALDO_A = (SALDO_A-PRECIO_BILLETE_JOVEN);
            cout << "Saldo actual: " << SALDO_A << endl;
        }
    }
    else
    {

        if (CLASE == "Universitario")
        {
            cout << "Nombre Completo: " << NOMBRE << endl;
            cout << "Precio del billete: ";
            cout << PRECIO << "€" << endl;
            if (SALDO_A < PRECIO_BILLETE_JOVEN ){
                cout << "Saldo actual: " << SALDO_A << endl;
                solicitud_recarga(0);
            }
            else 
            {
                SALDO_A = (SALDO_A-PRECIO_BILLETE_JOVEN);
                cout << "Saldo actual: " << SALDO_A << endl;
            }
        }

        else 
        {
            if (CLASE == "Ordinario")
            {
                cout << "Precio del billete: ";
                cout << PRECIO_BILLETE_ORDINARIO << "€" << endl;
                if (SALDO_A < PRECIO_BILLETE_ORDINARIO ){
                    cout << "Saldo actual: " << SALDO_A << endl;
                    solicitud_recarga(0);
                }
                else 
                {
                    SALDO_A = (SALDO_A-PRECIO_BILLETE_ORDINARIO);
                    cout << "Saldo actual: " << SALDO_A << endl;
                }
            }
            else
            {
                if (CLASE=="PersonasConDiscapacidad")
                {
                    cout << "Nombre Completo: " << NOMBRE << endl;
                    cout << "Precio del billete: ";
                    cout << PRECIO_BILLETE__DISCAPACITADOS << "€" << endl;
                    if (SALDO_A < PRECIO_BILLETE__DISCAPACITADOS ){
                    cout << "Saldo actual: " << SALDO_A << endl;
                    solicitud_recarga(0);
                    }
                    else 
                    {
                        SALDO_A = (SALDO_A-PRECIO_BILLETE__DISCAPACITADOS);
                        cout << "Saldo actual: " << SALDO_A << endl;
                    }

                }
                else{
                    if (CLASE=="Credibus")
                    {
                        cout << "Precio del billete: " << PRECIO << "€" <<endl;
                        if (SALDO_A < PRECIO ){
                            cout << "Saldo actual: " << SALDO_A << endl;
                            solicitud_recarga(0);
                        }
                        else 
                        {
                            SALDO_A = (SALDO_A-PRECIO);
                            cout << "Saldo actual: " << SALDO_A << endl;
                        }
                    
                    }else{

                        if (CLASE=="Pensionista")
                        {
                            cout << "Nombre Completo: " << NOMBRE << endl;
                            cout << "Precio del billete: ";
                            cout << PRECIO_BILLETE_PENSIONISTA << "€" << endl;
                            cout << "Saldo actual: " << SALDO_A << endl;

                        }
                    }  
                }

            }
        }
    }
}



void recargar(int cantidad)
{

    switch(cantidad)
    {
        case 5:
            PRECIO = PRECIO_BILLETE_CREDIBUS_5;
        break;
        case 10:
            PRECIO = PRECIO_BILLETE_CREDIBUS_10;
        break;
        case 20:
            PRECIO = PRECIO_BILLETE_CREDIBUS_20;
        break;
    }
    cout << "Recargando..." << endl;
    SALDO_A = (SALDO_A + cantidad);
    cout << "Saldo actual: " << SALDO_A<< endl;
    cout << "Vuelva a ticar." << endl;
    ticar();
    
    
}

void solicitud_recarga(int decision)
{
    switch (decision)
    {
        case 0:
            cout << "Saldo agotado. RECARGAR" << endl;
            cout << "¿Cuánto quiere recargar:? " << endl;
            
        break;

        case 1:
            cout << "Recargar credibus. ¿Cuánto quiere recargar? (5,10 ó 20 €): " << endl;
        break;

    }
    cin >> CANT_REC;
    while (CANT_REC!=5 && CANT_REC!=10 && CANT_REC!=20){
        cout << "No se puede recargar esta cantidad (solo 5,10, o 20), saque otro billete." << endl;
        cin  >> CANT_REC;
    }
    
    recargar(CANT_REC);
    
}

void actualizar_bonobus(FILE *pfile)
{
    string S = to_string(SALDO_A).substr(0,6);
    string P = to_string(PRECIO).substr(0,4);
    string fec = obtener_date_actual();
    string v[7] = {ID,S,P,CLASE,NOMBRE,LINEA,fec};

    fputs(const_cast<char*>(v[0].c_str()), pfile);
    fputs("\n", pfile);
    fputs(const_cast<char*>(v[1].c_str()), pfile);
    fputs("\n", pfile);      
    fputs(const_cast<char*>(v[2].c_str()), pfile);
    fputs("\n", pfile);
fputs(const_cast<char*>(v[3].c_str()), pfile);
    fputs("\n", pfile);
    if (CLASE != "Credibus" && CLASE != "Ordinario" ){
        fputs(const_cast<char*>(v[4].c_str()), pfile);
        fputs("\n", pfile);
    }
    fputs(const_cast<char*>(v[5].c_str()), pfile);
    fputs("\n", pfile);
    fputs(const_cast<char*>(v[6].c_str()), pfile);
    fputs("\n", pfile);
    fclose(pfile);
}   

void crear_bonobus()
{
    srand(time(NULL));
    ID      = to_string(10000000+rand()%(50000000-10000000));
    CLASE   = "Credibus";
    LINEA   = lineas[0+rand()%(28)];
    DATE    =  obtener_date_actual();    
}

string obtener_date_actual()
{
    auto start = std::chrono::system_clock::now(); 
    std::time_t date_hour = std::chrono::system_clock::to_time_t(start); 
    string cad = ctime(&date_hour);
    return cad;
}