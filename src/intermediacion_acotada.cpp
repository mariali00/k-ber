#include "intermediacion_acotada.h"


//! Construtor.
/*!
   *  Inicializa todos los atributos de la clase con los datos pasados como parametro.
*/
    intermediacion_acotada::intermediacion_acotada(igraph_t g,int n,igraph_vs_t v,igraph_bool_t d)
	{
		graph=g;
		k=n;
		vids=v;
		directed=d;
	}
/** \brief Imprime algunos datos relevantes del grafo que contiene el objeto
 *
 *  \return Es una funcion void, no retorna nada.
 *  La funcion imprime el diametro, la cantidad de aristas y de vertices del grafo usado en el algoritmo.
 *  Para ello utiliza funciones de igraph que dado el grafo devuelven los valores mencionados.
 */

    void intermediacion_acotada::estadistica()
    {
         //diametro del grafo
		 igraph_integer_t diam;
		 igraph_diameter(&graph,&diam,NULL,NULL,NULL,directed,true);
		 cout<<"Diametro: "<<diam<<endl;

		 //contador de arista
		 cout<<"Cantidad de arista: "<<igraph_ecount(&graph)<<endl;

		 //contador de vertices
		 cout<<"Cantidad de vertices: "<<igraph_vcount(&graph)<<endl;

         cout<<"Limite de distancia: "<<k<<endl;
    }

/** \brief Algoritmo de Ulrik Brandes para calcular la intermediacion para todos los vertices de un grafo.
 *
 *  \param [out] res: un vector de igraph no inicializado donde se guardaran las intermediaciones de los vertices deseados.
 *  \return Es una funcion void, no retorna nada.
 *  La funcion utiliza el algoritmo expuesto por Brandes en el articulo On variants of shortest-path betweenness
 *  centrality and their generic computation del 2008. Teniendo un grafo y los vertices a los que se le desea hallar
 *  la intermediacion en ell algoritmo se realiza un BFS para calcular los caminos mas cortos entre cualquiera dos vertices y
 *  luego  se acumulan esos resultados para calcular las dependecias por pares y mas tarde la intermediacion.
 *  En el calculo de los caminos mas cortos solo intervendran los caminos menores que una constante k.
 *  Se calcula  la intermediacion para todos los vertices pero solo se guardan en el parametro de salida la intermediacion
 *  de los vertices que se encuentran en vids y en el mismo orden en que aparecen, similar a la implementacion de este
 *  algoritmo en igraph.
 */

    void intermediacion_acotada::calcular (igraph_vector_t &res)
	{

		 //selector de vertices
		 igraph_vs_t allv;
		 igraph_vs_all (&allv);
		 igraph_vit_t allvit;

		 igraph_vit_create (&graph, allv, &allvit);

		 long long ctdad_v=IGRAPH_VIT_SIZE(allvit);
		 long double Cb[ctdad_v];
		 for(int i=0;i<ctdad_v;i++)
		 {
			 Cb[i]=0;
		 }
         queue <long long> Q;
         stack <long long> S;


		 while (!IGRAPH_VIT_END(allvit))
		 {

			long long s=IGRAPH_VIT_GET(allvit);

			//inicializacion
			list<long long> Pred[ctdad_v];
			long double  sigma[ctdad_v];
			long long dist[ctdad_v];
			long double delta[ctdad_v];//para acumulacion

			igraph_vit_t tvit;

			igraph_vit_create (&graph, allv, &tvit);

			while (!IGRAPH_VIT_END(tvit))
			{
				long long t=IGRAPH_VIT_GET(tvit);
				sigma[t]=0;
				dist[t]=-1;
				delta[t]=0;
				IGRAPH_VIT_NEXT(tvit);
			}

			dist[s]=0;
			sigma[s]=1;
			Q.push(s);

			while(!Q.empty())
			{
				long long v = Q.front();
				Q.pop();
				S.push(v);


				igraph_vs_t barrio_v;
				igraph_vs_adj (&barrio_v,v,IGRAPH_OUT);
				igraph_vit_t vit;

				igraph_vit_create (&graph, barrio_v, &vit);

				while (!IGRAPH_VIT_END(vit))
				{
					long long w=IGRAPH_VIT_GET(vit);


					if(dist[w]<0)
					{
						dist[w]=dist[v]+1;
						//caminos mas cortos menores que k
						if(dist[w]<=k)
						{
							Q.push(w);
						}
					}


					if(dist[w]==dist[v]+1)
					{
						sigma[w]+=sigma[v];

                        Pred[w].push_back(v);
					}
					IGRAPH_VIT_NEXT(vit);
				}
			}


                //acumulacion
				while(!S.empty())
				{
					long long w=S.top();
					S.pop();
                    for (auto it : Pred[w])
                    {
                        delta[it]+= (1+delta[w])*sigma[it]/sigma[w]  ;
                    }
					if(w!=s)
					{
						Cb[w]+=delta[w];
					}
				}

				IGRAPH_VIT_NEXT(allvit);

			}
            if(!igraph_is_directed(&graph))
            {
                for(int i=0;i<ctdad_v;i++)
                {
                   Cb[i]/=2;
                }
            }

            int c_res;
            igraph_vs_size(&graph,&vids,&c_res);
            igraph_vector_init (&res,c_res);
            igraph_vit_t vidst;
            igraph_vit_create (&graph, vids, &vidst);
            int j=0;
            while(!IGRAPH_VIT_END(vidst))
            {
                int i = IGRAPH_VIT_GET(vidst);
                VECTOR (res)[j]=Cb[i];
                IGRAPH_VIT_NEXT(vidst);
                j++;
            }

	}
