#include "pruebas_intermediacion.h"

    pruebas_intermediacion::pruebas_intermediacion()
    {
        //ctor
    }

/** \brief Compara los resultados del calculo de intermediacion con las bibliotecas intermediacion_acotada y igraph
 *
 *  \param [in] a: un objeto intermediacion_acotada inicializado para el calculo de intermediacion
 *  \return true: si los resultados de ambas bibliotecas son iguales o bastante semejantes o false si son diferentes.
 *  La funcion utiliza el parametro para calcular la intermediacion con las funciones que se encuentran en ambas
 *  bibliotecas, los resultados son guardados en vectores diferentes, uno para cada funcion. Los resultados se consideran
 *  correctos si para todos los vertices el valor absoluto de la resta de ambos resultados en menor que 10e-7.
 */

    bool pruebas_intermediacion::comparar_resultados_betweenness(igraph_t graph, igraph_vs_t vids )
	{
	    int c;
	    igraph_vs_size(&graph,&vids,&c);
	    bool directed=igraph_is_directed(&graph);
        int k;
        igraph_diameter(&graph,&k,NULL,NULL,NULL,directed,1);
	    igraph_vector_t res;
	    intermediacion_acotada a(graph,k,vids,directed);
	    a.calcular(res);
	    igraph_vector_t ires;
        igraph_vector_init (&ires,1);
        bool dirigido=0;
        if(igraph_is_directed(&graph))
         {
             dirigido=1;
         }
	    igraph_betweenness(&graph,&ires,vids,dirigido,NULL,true);
	    for(int i=0;i<c;i++)
        {
            if(abs(VECTOR(res)[i]-VECTOR(ires)[i])>10e-7)
            {
                return false;
            }
        }
        return true;
	}
/** \brief Solo se utiliza para que sea ejecutada y conocer el tiempo en que lo hace
 *
 *  \return Es una funcion void, no retorna nada.
 *  Crea un grafo aleatorio y lo guarda en un fichero. Luego llama a la funcion en intermediacion_acotada
 *  para calcular intermediacion
 */

	void pruebas_intermediacion::pruebas_tiempo_implementacion()
	{
	    igraph_t graph;
        igraph_vs_t vids;

         srand(time(0));
         freopen("prueba.txt", "w", stdout);
         int vert_arist=rand()%2000;
         if(vert_arist%2!=0)
         {
             vert_arist++;
         }
         int a[vert_arist];
         for(int i=0;i<vert_arist;i++)
         {
            a[i]=rand()%vert_arist;
            cout<<a[i]<<endl;
         }
         cout<<-1<<endl;
        igraph_vector_t c_v;
        igraph_vector_init (&c_v,vert_arist);
        for(int i=0;i<vert_arist;i++)
         {
            VECTOR (c_v) [i]=a[i];
         }

        bool directed=rand()%2;

        igraph_create(&graph,&c_v,0, directed);
        igraph_vs_all(&vids);
        igraph_integer_t diam;
        igraph_diameter(&graph,&diam,NULL,NULL,NULL,directed,1);

        int k=diam;
        /*if(diam!=0)
        {
            b.k=rand()%diam+1;
        }*/
        cout<<k<<endl;
        igraph_vector_t ires;
        intermediacion_acotada b(graph,k,vids,directed);
        b.calcular(ires);
        cout<<directed<<endl;
	}
/** \brief Solo se utiliza para que sea ejecutada y conocer el tiempo en que lo hace
 *
 *  \return Es una funcion void, no retorna nada.
 *  Toma el grafo de un fichero guardado por la funcion pruebas_tiempo_implementacion
 *  y ejecuta la funcion para calcular intermediacion de igraph. Imprime los estadistica del grafo.
 */
	void pruebas_intermediacion::pruebas_tiempo_igraph()
	{
	    igraph_t graph;
	    int k;
        igraph_vs_t vids;
        bool directed;
	    freopen("prueba.txt", "r", stdin);
	    igraph_vector_t ver;
         vector <long long> aux;
         long long a=1;
         while(a>=0)
         {
             cin>>a;
             if(a>=0)
             {
                aux.push_back(a);
             }
         }
         igraph_vector_init (&ver,aux.size());

         for(size_t s=0;s<aux.size();s++)
         {
             VECTOR(ver)[s]=aux[s];
         }


        cin>>k;

        cin>>directed;
        igraph_create(&graph,&ver,0, directed);
        igraph_vs_all(&vids);

        igraph_vector_t res;
        igraph_vector_init (&res,1);
        intermediacion_acotada b(graph,k,vids,true);
        b.estadistica();
        igraph_betweenness(&graph,&res,vids,directed,NULL,true);
	}

/** \brief Utiliza grafos aleatorios para comprobar la presicion del algoritmo en intermediacion_acotada
 *
 *  \return Es una funcion void. No retorna nada.
 *  Crea grafos aleatorios con el modelo Erdos-Renyi y llama a la funcion comparar_resultados_betweenness que
 *  compara los resultados del calculo de intermediacion con las bibliotecas intermediacion_acotada y igraph.
 *  de todos los grafos generados imprime la cantidad que fueron correctos y la cantidad que fueron incorrectos.
 */

    void pruebas_intermediacion::grafos_aleat_igraph_precision()
    {
        srand(time(0));
        int correctos=0;
        int incorrectos=0;
        bool res;
        for(int i=1;i<26;i++)
        {

            igraph_t graph;
            int k;
            igraph_vs_t vids;
            igraph_bool_t directed=rand()%2;

            for(int j=2* i;j<9 * i && j < (i * (i - 1) / 4); j+= i)
            {
                for(int t =0 ; t < 100; t++){
                    igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNM,i,j,directed,true);
                    igraph_vs_all(&vids);
                    igraph_diameter(&graph,&k,NULL,NULL,NULL,directed,true);
                    intermediacion_acotada b(graph,k,vids,directed);
                    res=comparar_resultados_betweenness(graph,vids);
                    if(res)
                        correctos++;
                    else incorrectos++;
                }
            }

        }
        cout<<"Fueron procesados "<<correctos<<" grafos con resultados correctos"<<endl;
        cout<<"Fueron procesados "<<incorrectos<<" grafos con resultados incorrectos"<<endl;
    }
/** \brief Toma grafos de archivos y comprueba que la intermediacion de intermediacion_acotada este correcta
 *
 *  \param [in] archivo: el nombre y ruta del archivo a leer
 *  \return Es una funcion void, no retorna nada.
 *  Toma grafos de archivos y llama a la funcion comparar_resultados_betweenness si esta devuelve verdadero
 *  imprime que los resultados fueron correctos y en caso contrario imprime que fueron incorrectos.
 */

    void pruebas_intermediacion:: pruebas_grafos_leidos(char* archivo)
    {
        igraph_t graph;
         freopen(archivo, "r", stdin);
         igraph_vector_t ver;
         vector <long long> aux;
         long long a=1;
         while(a>=0)
         {
             cin>>a;a--;
             if(a>=0)
             {
                aux.push_back(a);
             }
         }
         igraph_vector_init (&ver,aux.size());

         for(size_t s=0;s<aux.size();s++)
         {
             VECTOR(ver)[s]=aux[s];
         }

         int k;
         igraph_bool_t directed;
         cin>>directed;
        igraph_create(&graph,&ver,0, directed);
        igraph_diameter(&graph,&k,NULL,NULL,NULL,directed,1);
        igraph_vs_t vids;
	    igraph_vs_all(&vids);
         intermediacion_acotada b (graph,k,vids,directed);
         b.estadistica();

         if(comparar_resultados_betweenness(graph,vids))
         {
             cout<<"Resultados correctos para el grafo leido"<<endl;
         }
         else cout<<"Resultados incorrectos para el grafo leido"<<endl;
    }
/** \brief Con grafos aleatorios guarda solo en los vectores la intermediacion de algunos vertices
 *
 *  \return  Es una funcion void, no retorna nada.
 *  Crea grafos aleatorios con el modelo Erdos-Renyi. El atributo vids que guarda los ids de los vertices
 *  que se desea conocer su intermediacion puede no contener todos los vertices del grafo. La funcion demuestra que
 *  para casos como este la implementacion en  intermediacion_acotada devuelve el mismo vector que la implementacion en igraph.
 */

    void pruebas_intermediacion::intermediacion_algunos_vertices()
    {
         srand(time(0));
         int c_v=rand()%2000;
         int c_a=rand()%2000;
         bool dirigido=rand()%2;

         int n;
         cout<<"Entre la cantidad de vertices de los cuales conocera su intermediacion:"<<endl;
         cout<<"Dicha cantidad debe ser menor que "<<c_v<<endl;
         cin>>n;
         igraph_vector_t v;
         igraph_vector_init (&v,n);
         cout<<"Entre los "<<n<<" vertices deseados menores que "<<c_v<<":"<<endl;
         for(int i=0;i<n;i++)
         {
             cin>>VECTOR(v)[i];
         }
         igraph_vs_t id;
         igraph_vs_vector_copy (&id,&v);
         igraph_vector_t res;


         igraph_vector_init (&res,c_v);
         igraph_t graph;
         igraph_erdos_renyi_game(&graph, IGRAPH_ERDOS_RENYI_GNM,c_v,c_a,dirigido,true);


        igraph_betweenness(&graph,&res,id,dirigido,NULL,true);

        int k;
        igraph_diameter(&graph,&k,NULL,NULL,NULL,dirigido,true);
        intermediacion_acotada a(graph,k,id,dirigido);
        igraph_vector_t ires;
        a.calcular(ires);

         for(int i=0;i<n;i++)
         {
            cout<<VECTOR(res)[i]<<" ";
         }
         cout<<endl;
         for(int i=0;i<n;i++)
         {
            cout<<VECTOR(ires)[i]<<" ";
         }
         cout<<endl;
        if(comparar_resultados_betweenness(graph,id))
        {
            cout<<"Resultados correctos"<<endl;
        }
        else cout<<"Resultados incorrectos"<<endl;
    }
