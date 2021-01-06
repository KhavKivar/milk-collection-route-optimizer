#include <iostream>
#include <fstream>
#include <string>
#include <map> 
#include <bits/stdc++.h> 
#include <queue>
#include  <random>

using namespace std;


//VARIABLES DE PARAMETROS
//
//
//
//

//demanda total por tipo de leche
vector<int> demanda_tipo;


//ganancia por tipo de leche
vector<double> ganancia_tipo;


//capacidad de cada camion
vector<int> capacidad_camion;



//numero de camiones
int camiones;
//numero de tipos de leche
int tipo_leche;
//numero de nodos
int numero_nodos;


//coordenadas de los nodos
vector<pair<int,int>> coordenada;

//produccion de leche por nodo
vector<pair<int,int>> produccionXNodo;


//Tipo de leche to int
map<string,int> tipo_leche_to_int;
//Tipo de leche to string
map<int,string> tipo_leche_to_string;


vector<vector<int>> lista_tipoLeche;



vector<string> str_split(string input){
    vector<string> tokens; 
    stringstream check1(input); 
    string intermediate; 
    while(getline(check1, intermediate,'	')) 
    { 
        tokens.push_back(intermediate); 
    } 
    return tokens;
}





void print_input(){
    //print Camiones
    cout << "Camiones:" << endl;
    for (auto const& x : capacidad_camion){
        cout << x << endl ;
    }

    cout << "Tipos de leche: "<< endl;
    //print tipos de leche
     for (auto const& x : demanda_tipo){
        cout << x << endl ;
    }


    //print Nodos
    cout << "Nodo Info: "<< endl;

    for(int i=0;i<numero_nodos;i++){
        cout << "Nodo Numero "<<  i << endl;
        cout << "x: "<< coordenada[i].first << " y: "<< coordenada[i].second << endl;
         cout << "Produccion tipo de leche:  " << produccionXNodo[i].first << " Suply: " <<produccionXNodo[i].second <<  endl;
    }
}





void leer_input(string path){

    tipo_leche_to_int.insert(pair<string,int>("A",0) );
    tipo_leche_to_int.insert(pair<string,int>("B",1) );
    tipo_leche_to_int.insert(pair<string,int>("C",2) );

    tipo_leche_to_string.insert(pair<int,string>(0,"A"));
    tipo_leche_to_string.insert(pair<int,string>(1,"B") );
    tipo_leche_to_string.insert(pair<int,string>(2,"C") );


    string line;
    ifstream myfile(path);
    
    bool first_line = false;
    bool capacidades_bool =false;
    bool second_line = false;
    int contador = 0;

    if (myfile.is_open()){
        while ( getline (myfile,line) ) {
            vector<string> ipt = str_split(line);
       
            

            //Primera Linea
            if(contador == 0){
                camiones = atoi(ipt[0].c_str());
                capacidad_camion.resize(camiones,-1);
            }
            if(contador == 3){
                tipo_leche = atoi(ipt[0].c_str());
                ganancia_tipo.resize(tipo_leche,-1);
                demanda_tipo.resize(tipo_leche,-1);
                lista_tipoLeche.resize(3);

            }
            if(contador == 7 ){
                numero_nodos = atoi(ipt[0].c_str());

            }

            if ( contador < 7){
                for(int i = 0; i < ipt.size(); i++) {
                    if(contador == 1){
                        capacidad_camion[i] = atoi(ipt[i].c_str());
                    }   
                    if(contador == 4){
                        demanda_tipo[i] = atoi(ipt[i].c_str());
                    }
                    if( contador == 5){
                        ganancia_tipo[i] = stold(ipt[i]);
                    }
                }
            }


            if (contador>7){
                pair<int, int>  xy;
                xy = make_pair(atoi(ipt[1].c_str()),atoi(ipt[2].c_str()));
                coordenada.push_back(xy);
                auto tipo_leche_nodo = tipo_leche_to_int[ipt[3]];


                if(ipt[3] != "-"){
                     int nodo_xy = atoi(ipt[0].c_str());
                     nodo_xy = nodo_xy-1;
                     lista_tipoLeche[tipo_leche_nodo].push_back(nodo_xy);
                }
              
                

                xy = make_pair(tipo_leche_nodo,atoi(ipt[4].c_str()) );
                produccionXNodo.push_back(xy);
                
                    
                  
            }
  

            contador++;
        }
        myfile.close();
    }
}





//VARIABLES DEL ALGORITMO
//
//
//
//


double ** MATRIX_DISTANCE;

//Solucion Actual

//Lista de nodos separadas por calidad





bool value_in_queeue(queue<int> cola,int value){
    while(!cola.empty()){
        int x = cola.front();
        if(x == value){
            return true;
        }
        cola.pop();
	}
    return false;
}



queue<int> init_queue_camiones(){
    
    queue<int> cola_camiones;
    int capacidad_mayor = -1;
    int pos = -1;
    for (int i = 0; i < camiones; i++){
        for (int j = 0; j < camiones; j++) {
            int capacidad = capacidad_camion[j];
            if(capacidad>= capacidad_mayor && !value_in_queeue(cola_camiones,j)){
                capacidad_mayor = capacidad;
                pos = j;
            }
        }   
        if(capacidad_mayor != -1){
            cola_camiones.push(pos);
            capacidad_mayor = -1;
            pos =-1;
        }
    }
    return cola_camiones;
}


void init_matrix_distancia(){
    int nodos = coordenada.size();
    MATRIX_DISTANCE = new double*[nodos];
    for (int i = 0; i < nodos; ++i) {
           MATRIX_DISTANCE[i] = new double[nodos];
    }
}

double distancia(pair<int,int> p1, pair<int,int> p2) {
    return sqrt(pow((p2.second-p1.second),2)+ pow((p2.first-p1.first),2));
}

void calcular_distancia(){
    for (int i = 0; i < coordenada.size(); i++){
        for (int j = 0; j < coordenada.size(); j++){
            if(i == j){
                MATRIX_DISTANCE[i][i] = 0;
                continue;
            }   
            MATRIX_DISTANCE[i][j] = distancia(coordenada[i],coordenada[j]);
        }
    }
}





bool value_in_lista(vector<int> lista,int value){
    return find(lista.begin(),lista.end(),value) != lista.end();
}
bool sortbysec(const pair<int,double> &a, 
              const pair<int,double> &b) 
{ 
    return (a.second < b.second); 
} 

vector<int> lista_nodos_mas_cercanos(vector<int> nodos_factibles,int nodoOrigen,int largo){
    vector<pair<int,double>> lista_para_ordenar;
    for (int i = 0; i < coordenada.size(); i++){
        if(nodoOrigen == i){
            continue;
        }
        if(value_in_lista(nodos_factibles,i)){
            double xy = MATRIX_DISTANCE[nodoOrigen][i];
            lista_para_ordenar.push_back(make_pair(i,xy));
        }
    }

    //Ordenamos de mayor a menor
    sort(lista_para_ordenar.begin(),lista_para_ordenar.end(),sortbysec);

    vector<int> lista_final;
    for (int i = 0; i < largo; i++) {
        if(i >= lista_para_ordenar.size()){
            break;
        }
        lista_final.push_back(lista_para_ordenar[i].first);
    }
    return lista_final;
}


void print_matrix(int fila){
    cout << "fila " << fila<<endl;
    for(int i=0;i < coordenada.size();++i){
        cout <<MATRIX_DISTANCE[fila][i] << " ";
    }
    cout << endl;
}



vector<int> ListaCandidata(int nodo,int tipo_de_leche,vector<int> lista_nodos_usados,int capacidad_actual,int capCamion,int largoLista = 3){
    //Generamos una lista con los nodos con tipo de leche == tipo_de_leche o superior
    vector<int> Lista;
    int contador = 0;
    while(contador <= tipo_de_leche ){        
        Lista.insert(Lista.end(),lista_tipoLeche[contador].begin(),lista_tipoLeche[contador].end());
        contador++;
    }

    //Limpiar la lista resultante;
    vector<int> lista_final;

    if(lista_nodos_usados.size() == 0){
        return Lista;
    }
    
    for (int i = 0; i < Lista.size(); i++){
        int produccion_nodo = produccionXNodo[Lista[i]].second;
        if(value_in_lista(lista_nodos_usados,Lista[i]) || (capacidad_actual+produccion_nodo)>capCamion ){
            continue;
        }
        lista_final.push_back(Lista[i]);
    }
   
    //Cortamos la lista solo con los mas cercanos al nodo
    vector<int> candidado_finales;
    if (nodo != -1 && lista_final.size() > 0){
        candidado_finales = lista_nodos_mas_cercanos(lista_final,nodo,largoLista);
        return candidado_finales;
    }

    return lista_final;

}


int chooseRandom(vector<int> candidatos,int tipo_leche){
    vector<double> probabilidades;
    int suma = 0;
    int factor = 10;
    for(auto x: candidatos){
        int produccion = produccionXNodo[x].second;
        if(produccionXNodo[x].first < tipo_leche){
            //Aumentamos la probabilidad
            produccion = produccion*factor;
            suma += produccion;
            probabilidades.push_back(produccion);
        }else{
             suma += produccion;
             probabilidades.push_back(produccion);
        }
        
       
    }
    for (int i = 0; i <probabilidades.size(); i++){
        probabilidades[i] = probabilidades[i]/suma;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(probabilidades.begin(),probabilidades.end());

    int random = d(gen);

    return candidatos[random];
}


vector<vector<int>> solucion_inicial(){
    //Se genera una cola de camiones ordenada segun su capacidad    
    queue<int> camiones_cola = init_queue_camiones();

    //inicializamos la matrix
    init_matrix_distancia();
    //Calculamos la matrix de distancia
    calcular_distancia();
    
    vector<vector<int>> solucion_inicial;


    solucion_inicial.resize(camiones);

    int tipolecheChoosen = 0;

    vector<int> nodos_usados;


    while(!camiones_cola.empty()){
        int current_camion = camiones_cola.front();
        int capacidad = capacidad_camion[current_camion];
        int nodo;
        int capacidad_actual = 0;

        while(capacidad_actual < capacidad){
            if(solucion_inicial[current_camion].size() == 0){
                nodo = -1;
                 cout << "camion" << current_camion<<endl;
            }else{
                nodo = solucion_inicial[current_camion].back();
            }
           
            vector<int> candidatos = ListaCandidata(nodo,tipolecheChoosen,nodos_usados,capacidad_actual,capacidad);
            
            if (candidatos.size() == 0){
              
                break;
            }
            cout <<"Candidatos : " ;
            for(auto x:candidatos){
                cout << " " << x ;
            }

            int candidato = chooseRandom(candidatos,tipolecheChoosen);
            cout << " choosen: " << candidato <<endl;

            
            capacidad_actual+= produccionXNodo[candidato].second;
            cout << "Capacidad actual:  " << capacidad_actual << endl;
            solucion_inicial[current_camion].push_back(candidato);
            nodos_usados.push_back(candidato);
        }
        tipolecheChoosen++;
        camiones_cola.pop();
        


    }
    return solucion_inicial;

    


}


double get_cost_per_distance(int anterior, int actual){
    return MATRIX_DISTANCE[anterior][actual];
}

double costo_por_ruta(vector<int> ruta){
    double coste = 0;
    int anterior = 0;
    for(auto x:ruta){
        coste += get_cost_per_distance(anterior,x);
        anterior = x;
    }
    coste+= get_cost_per_distance(anterior,0);
    return coste;

    
}

int highest_type_of_milkd(vector<int> ruta){
    int highest_type_of_milk=-1;
    for(auto x: ruta){
        int type_milk = produccionXNodo[x].first;
        if(type_milk > highest_type_of_milk){
            highest_type_of_milk = type_milk;
        }
    }
    return highest_type_of_milk;
}

int ganancia_por_ruta(vector<int> ruta){
    double ganancia=0;
   
    int highest_type_of_milk = highest_type_of_milkd(ruta);

    for(auto x:ruta){
        ganancia += produccionXNodo[x].second*ganancia_tipo[highest_type_of_milk];
    }
    return ganancia;
}

struct resultado{
    double objectivo;
    double ganancia;
    double coste;
};

resultado funcion_objetivo(vector<vector<int>> solucion){
    
    double obj=0,coste=0,ganancia=0;
    resultado tipleta;
    
    for (int i = 0; i < solucion.size(); i++){
        coste += costo_por_ruta(solucion[i]);
        ganancia += ganancia_por_ruta(solucion[i]);
    }
    tipleta.coste = coste;
    tipleta.ganancia = ganancia;
    tipleta.objectivo = ganancia-coste;

    return tipleta;

    
    //Costos de transporte
    //Ganancia por litro leche
    
}

void print_solucion(vector<vector<int>> solucion){
    //Calcular 
    resultado x = funcion_objetivo(solucion);
    cout << "SOLUCION------------------------------" <<endl;
    cout << x.objectivo << " " << x.coste << " "<< x.ganancia << endl<<endl;
    for (int i = 0; i < solucion.size(); i++){
        for (int j = 0; j < solucion[i].size(); j++){
            cout << solucion[i][j];
           
            if(j+1 == (solucion[i].size())){
                cout << " ";
                continue;
            }
            cout << "-";
        }
        cout << costo_por_ruta(solucion[i]) << " " << ganancia_por_ruta(solucion[i]) <<" " << tipo_leche_to_string[highest_type_of_milkd(solucion[i])] << endl;
    }

    

}



int main(int argc, char *argv[]) {
    leer_input(argv[1]);
    int iteraciones = atoi(argv[2]);
    
    vector<vector<int>> sol = solucion_inicial();
    print_solucion(sol);



    
    
}

