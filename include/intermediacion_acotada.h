#ifndef INTERMEDIACION_ACOTADA_H
#define INTERMEDIACION_ACOTADA_H
#include <igraph/igraph.h>
#include <iostream>
#include <queue>
#include <stack>
#include <list>
using namespace std;

//!Clase que calcula la intermediacion de todos los vertices de un grafo
/*!
   *  Esta clase permite crear objetos con los que utilizando sus atributos es suficiente para calcular
   *  la intermediacion de todos los vertices de un grafo. En dicha intemediacion solo influyen los caminos
   *  con distancia menores o iguales que un limite k. Esta clase permite el acceso a sus atributos privados
   *  a la clase algorithm_test que se encargara de comprobar los resultados del algoritmo.
   *  \author    Mariali Guzmán
   *  \version   1.0a
   *  \date      2019-7-3
*/
class intermediacion_acotada
{
private:
/*!
   *  graph: elemento de tipo grafo de la biblioteca igraph, que guarda el grafo que se utilizara en el
   *  algoritmo.
*/
	igraph_t graph;
/*!
   *  k: limite de distancia para los caminos que influiran en el calculo de la intermediacion de un vertice.
   *  Es contante para todos los vertices.
*/
	int k;
/*!
   *  vids: elemento de tipo selector de vertice que guarda los ids de los vertices que se desea conocer su
   * intermediacion
*/
	igraph_vs_t vids;
/*!
   *  directed: elemento booleano que guarda si en grafo es dirigido o no con 1 o 0 respectivamente.
*/
	igraph_bool_t directed;

public:
	intermediacion_acotada(igraph_t, int, igraph_vs_t, igraph_bool_t  );
    void estadistica();
    void calcular(igraph_vector_t& res);
};


#endif // INTERMEDIACION_ACOTADA_H
