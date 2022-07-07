#include "intermediacion_acotada.h"
#include "pruebas_intermediacion.h"

using namespace std;


int main()
{
     pruebas_intermediacion test;
     /*char archivo[] = "CollegeMsg.txt";
     test.pruebas_grafos_leidos(archivo);*/
     //test.pruebas_tiempo_implementacion();
     //test.pruebas_tiempo_igraph();
     test.grafos_aleat_igraph_precision();
     //test.intermediacion_algunos_vertices();

     return 0;
}
