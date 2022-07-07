#ifndef PRUEBAS_INTERMEDIACION_H
#define PRUEBAS_INTERMEDIACION_H
#include "intermediacion_acotada.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

//!Clase que compara los resultados del calculo de intermediacion de las bibliotecas intermediacion_acotada y igraph
/*!
   *  Esta clase permite saber si los resultados fueron correctos utilizando ambas bibliotecas, ademas de comparar
   *  los tiempos de ejecucion de las funciones que claculan intermediacion para los mismos grafos en ambas bibliotecas.
   *  Para ello usa grafos aleatorios y otros tomados desde archivos. La clase es amiga de
   *  la clase intermediacion_acotada por lo que puede acceder a sus datos privados.
   *  \author    Mariali Guzmán
   *  \version   1.0a
   *  \date      2019-7-3
*/
class pruebas_intermediacion
{
    public:
        pruebas_intermediacion();
        bool comparar_resultados_betweenness(igraph_t, igraph_vs_t );
        void pruebas_tiempo_implementacion();
        void pruebas_tiempo_igraph();
        void grafos_aleat_igraph_precision();
        void pruebas_grafos_leidos(char* archivo);
        void intermediacion_algunos_vertices();

};

#endif // PRUEBAS_INTERMEDIACION_H
