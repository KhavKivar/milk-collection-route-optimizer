#include <iostream>
#include <fstream>
#include <string>
#include <map> 
#include <bits/stdc++.h> 
#include <queue>
#include  <random>
#include <ctime>


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

bool value_in_deque(deque<int> cola,int value){
    while(!cola.empty()){
        int x = cola.front();
        if(x == value){
            return true;
        }
        cola.pop_front();
	}
    return false;
}




deque<int> init_queue_camiones(){
    
    deque<int> cola_camiones;
    int capacidad_mayor = -1;
    int pos = -1;
    for (int i = 0; i < camiones; i++){
        for (int j = 0; j < camiones; j++) {
            int capacidad = capacidad_camion[j];
            if(capacidad>= capacidad_mayor && !value_in_deque(cola_camiones,j)){
                capacidad_mayor = capacidad;
                pos = j;
            }
        }   
        if(capacidad_mayor != -1){
            cola_camiones.push_back(pos);
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



vector<int> ListaCandidata(int nodo,int tipo_de_leche,vector<int> lista_nodos_usados,int capacidad_actual,int capCamion,int largoLista = 10){
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
    int factor = 0.1;
    for(auto x: candidatos){
        int produccion = produccionXNodo[x].second;
        if(produccionXNodo[x].first < tipo_leche){
            
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


vector<int> suplyTotal(){
   vector<int> sum_suply;
   sum_suply.resize(3,-1); 
    for (int i = 0; i < lista_tipoLeche.size(); i++) {
        int c = 0;
        for(auto x:lista_tipoLeche[i]){
            c += produccionXNodo[x].second;
        }
        sum_suply[i] = c;
    }
    return sum_suply;
}



int get_supply_per_route(vector<int> route){
    int suply= 0;
    for(auto x:route){
        suply+=produccionXNodo[x].second;
    }
    return suply;
}

bool cumple_con_la_demanda(deque<int> camiones_cola){
    for (int i = 0; i < 3; i++) {
        int c_capacidad = capacidad_camion[camiones_cola[i]];
        int d_tipo = demanda_tipo[i] ;  
        if ( c_capacidad<d_tipo ){
            return false;
        }
    }
    return true;
}

bool sortbyascn(const pair<int,double> &a, 
              const pair<int,double> &b) 
{ 
    return (a.second < b.second); 
} 

int choose_the_lowest(vector<pair<int,int>> x,int value){
    for (int i = 0; i < x.size(); i++){
        if(x[i].second>=value){
            return i;
        }
    }
    return -1;
}

deque<int> reorder_cola(deque<int> camiones_cola){
    deque<int> dummy;
    vector<pair<int,int>> cap_camiones;
    cap_camiones.resize(3,make_pair(-1,-1));
    for (int i = 0; i < 3; i++) {
       cap_camiones[i] = make_pair(i,capacidad_camion[i]);
    }
    sort(cap_camiones.begin(),cap_camiones.end(),sortbyascn);
    for (int i = 0; i < 3; i++){
        int c = choose_the_lowest(cap_camiones,demanda_tipo[i]);   
        cout <<"c: "<<c << "cap "<<cap_camiones[c].second<<endl;
        dummy.push_back(cap_camiones[c].first);
        cap_camiones.erase(cap_camiones.begin()+c);

    }
    
    return dummy;


}

vector<vector<int>> solucion_inicial(){
    //Se genera una cola de camiones ordenada segun su capacidad    
    deque<int> camiones_cola = init_queue_camiones();
    //Verificamos que cumpla con los requisitos capacidad del camion > demanda de tipo de leche
    if(!cumple_con_la_demanda(camiones_cola)){
        //Reordernar los nodos
        camiones_cola = reorder_cola(camiones_cola);
    }   

  


    vector<int> suply = suplyTotal();
    

    if (capacidad_camion[camiones_cola[2]] < suply[2]){
        if(capacidad_camion[camiones_cola[1]] >= suply[2] ){
            int value = camiones_cola[1];
            camiones_cola[1] = camiones_cola[2]; 
            camiones_cola[2] = value;
        }else{
              int value = camiones_cola[0];
            camiones_cola[0] = camiones_cola[2]; 
            camiones_cola[2] = value;
        }
    } 




    deque<int> copy_camiones_cola(camiones_cola);



    
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

        int demanda_leche = demanda_tipo[tipolecheChoosen];
        int suply_demanda = 0;
        while(suply_demanda < demanda_leche){
            if(solucion_inicial[current_camion].size() == 0){
                nodo = -1;
                 
            }else{
                nodo = solucion_inicial[current_camion].back();
            }
           
            vector<int> candidatos = ListaCandidata(nodo,tipolecheChoosen,nodos_usados,capacidad_actual,capacidad);
            if (candidatos.size() == 0){
                break;
            }
            int candidato = chooseRandom(candidatos,tipolecheChoosen); 
            suply_demanda+= produccionXNodo[candidato].second;
            capacidad_actual+= produccionXNodo[candidato].second;
            solucion_inicial[current_camion].push_back(candidato);
            nodos_usados.push_back(candidato);
        }
        tipolecheChoosen++;
        camiones_cola.pop_front();
    }



    //Reparar Solucion
    
    tipolecheChoosen = 0;

    while(!copy_camiones_cola.empty()){
        int current_camion = copy_camiones_cola.front();
        int capacidad = capacidad_camion[current_camion];
        int nodo;
        
        int capacidad_actual = get_supply_per_route(solucion_inicial[current_camion]);
        int demanda_leche = demanda_tipo[tipolecheChoosen];
        int suply_demanda = capacidad_actual;
        while(true){
            if(solucion_inicial[current_camion].size() == 0){   
                nodo = -1;
                 
            }else{
                nodo = solucion_inicial[current_camion].back();
            }
           
            vector<int> candidatos = ListaCandidata(nodo,tipolecheChoosen,nodos_usados,capacidad_actual,capacidad);
            if (candidatos.size() == 0){
                break;
            }
            int candidato = chooseRandom(candidatos,tipolecheChoosen); 
            suply_demanda+= produccionXNodo[candidato].second;
            capacidad_actual+= produccionXNodo[candidato].second;
            solucion_inicial[current_camion].push_back(candidato);
            nodos_usados.push_back(candidato);
        }
        tipolecheChoosen++;
        copy_camiones_cola.pop_front();
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

    
  
    
}

struct solucion_especial{
    vector<int> supply_milk;
    double profit;
    double cost;
    bool factible;
};



solucion_especial reparar_solucion(vector<vector<int>> sol,double costo){
    solucion_especial S0;
    S0.factible = false;
    S0.cost = costo;
    S0.supply_milk.resize(3,-1);

    vector<bool> tipo_leche_factible(3,-1);
    vector<int> supply_por_tipo(3,0);

    for (int i = 0; i < sol.size(); i++) {
        int leche_que_transporta = highest_type_of_milkd(sol[i]);
        int supply_total = get_supply_per_route(sol[i]);
        supply_por_tipo[leche_que_transporta] += supply_total;
    }
    S0.supply_milk = supply_por_tipo;

    for (int i = 0; i < supply_por_tipo.size(); i++){
        if(supply_por_tipo[i] < demanda_tipo[i] ){
            tipo_leche_factible[i] = false;
        }else{
             tipo_leche_factible[i] = true;
        }
    }
    for (int i = 0; i < tipo_leche_factible.size(); i++) {
        if(!tipo_leche_factible[i] && i > 0){
            int leche_tipo = i-1;
            bool terminar = true;
            while(leche_tipo > -1 && terminar){
                int faltante = demanda_tipo[i] -supply_por_tipo[i];
                //Verifico en las otras leche;
                
                int sobrante_vecino = supply_por_tipo[leche_tipo] - demanda_tipo[leche_tipo];
                if(sobrante_vecino >0){
                    int resultado = sobrante_vecino - faltante;
                    if(resultado >= 0 ){
                       //quedo satisfecho  
                        S0.supply_milk[i] = S0.supply_milk[i] + faltante;
                        S0.supply_milk[leche_tipo] = S0.supply_milk[leche_tipo] -faltante ;
                        terminar = false;
                        break;
                    }else{
                        //le robo todo al vecino
                        S0.supply_milk[i] = S0.supply_milk[i] + sobrante_vecino;
                        S0.supply_milk[leche_tipo] = S0.supply_milk[leche_tipo] - sobrante_vecino;
                    }
                }
                leche_tipo= leche_tipo-1;
            }
        }
    }
    //verifico si es factible y calculo el nuevo profit
    double profit = 0;
    S0.factible =true;
    for (int i = 0; i < S0.supply_milk.size(); i++){
        profit += S0.supply_milk[i] * ganancia_tipo[i];
        if(S0.supply_milk[i] < demanda_tipo[i]){
            S0.factible =false;
        }
    }
    
    S0.profit = profit;
  
   
    

    return S0;
    
    
        

}





bool es_factible(vector<vector<int>> sol){
    
    vector<int> supply_milk(3,0);

    for (int i = 0; i < sol.size(); i++){
        int leche_que_transporta = highest_type_of_milkd(sol[i]);
        int supply_total = get_supply_per_route(sol[i]);
        if(supply_total > capacidad_camion[i] ){
            return false;
        }
        supply_milk[leche_que_transporta] = supply_milk[leche_que_transporta] +supply_total;
    }
    //check
    for (int i = 0; i < 3; i++) {
        if(supply_milk[i] < demanda_tipo[i]){
            return false;
        }
    }
    
    
    return true;

}





void print_solucion(vector<vector<int>> solucion,double timer = -1){

    resultado x = funcion_objetivo(solucion);
    //Calcular 
   if(es_factible(solucion)){
      cout << "SOLUCION------------------------------" <<endl;
      cout << x.objectivo << " " << x.coste << " "<< x.ganancia << endl <<endl;
   }else{
       solucion_especial rp = reparar_solucion(solucion,x.coste);
       if(rp.factible){
          double pro = rp.profit-rp.cost;
          cout << "SOLUCION------------------------------" <<endl;
          cout << pro << " " << rp.cost << " "<< rp.profit << endl <<endl;
       }
   }
        
 


    int recolectado = 0;
    int all = 0;
    for (int i = 0; i < solucion.size(); i++){
         recolectado = 0;
        for (int j = 0; j < solucion[i].size(); j++){
            cout << (solucion[i][j]+1);
            recolectado += produccionXNodo[solucion[i][j]].second;
            if(j+1 == (solucion[i].size())){
                cout << " ";
                continue;
            }
            cout << "-";
        }
        all+= recolectado;
        cout << costo_por_ruta(solucion[i]) << " " << recolectado <<" " << tipo_leche_to_string[highest_type_of_milkd(solucion[i])] << endl;
    }
    if(timer > 0){
        cout << "Tiempo total de ejecucion: " << timer << "[s]" <<endl;
    }
    cout << "Leche recolectada: "<<all << endl;

    

}



vector<vector<int>> intra_ruta(vector<vector<int>> solucion_actual){
    
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,2);
    int rutaChoosen = dist6(rng);
    //2 opt
    int sizeRuta = solucion_actual[rutaChoosen].size();
  

    std::uniform_int_distribution<std::mt19937::result_type> distNodo(0,(sizeRuta-1));
    //Choose two number to swap
    int one = distNodo(rng);
    int two;
    while(true){
        two = distNodo(rng);
        if(two != one){
            break;
        }
    }
    
    
    vector<vector<int>> newMove(solucion_actual);
       
    int saveValue = newMove[rutaChoosen][one];
   
    newMove[rutaChoosen][one] = newMove[rutaChoosen][two];
    newMove[rutaChoosen][two] = saveValue;
   
   
    return newMove;
}



vector<vector<int>> extra_ruta(vector<vector<int>> solucion_actual){
     std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,2);
    int rutaChoosen = dist6(rng);
    int rutaChoosen2 = dist6(rng);

  
    //2 opt between route
    int size_route_one = solucion_actual[rutaChoosen].size();
    int size_route_two  = solucion_actual[rutaChoosen2].size();

 
    std::uniform_int_distribution<std::mt19937::result_type> distOne(0,(size_route_one-1));
    int choosenOne = distOne(rng);

  
    std::uniform_int_distribution<std::mt19937::result_type> distTwo(0,(size_route_two-1));

    int choosenTwo;
    if(rutaChoosen == rutaChoosen2){
        while (true) {
            choosenTwo = distTwo(rng);
            if(choosenOne != choosenTwo){
                break;
            }
        }
    }else{
        choosenTwo = distTwo(rng);
    }

  

    //2 opt
    int saveValue = solucion_actual[rutaChoosen][choosenOne];

    solucion_actual[rutaChoosen][choosenOne] = solucion_actual[rutaChoosen2][choosenTwo];
    solucion_actual[rutaChoosen2][choosenTwo] = saveValue;

    return solucion_actual;
}


vector<vector<int>> move(vector<vector<int>> solucion_actual){
    return extra_ruta(solucion_actual);


};






double factor_penalizacion;
double factor_sobrecuota;

double evaluar_calidad(vector<vector<int>> sol){
    double profit = 0;
    double cost = 0;
    double quality=0;
    for (int i = 0; i < sol.size(); i++) {
            profit+= ganancia_por_ruta(sol[i]);
            cost+= costo_por_ruta(sol[i]);
        }
    quality = profit-cost;
    
    if(es_factible(sol)){
        //Calcular el profit y el coste  
        return quality;    
    }else{
        //Verificamos si se puede mezclar la leche
        solucion_especial resp = reparar_solucion(sol,cost);
        profit = resp.profit;
        cost = resp.cost;
        quality = profit - cost;
        
        int cuota_faltante = 0;
        if(!resp.factible){
            for (int i = 0; i < 3; i++){
                int supply_type = resp.supply_milk[i];
                if(supply_type < demanda_tipo[i]){
                    cuota_faltante += demanda_tipo[i]-supply_type;
                }
            }
        } 
         //Si se excede el camion
        double sobre_exceso = 0;
        for (int i = 0; i < sol.size(); i++) {
            int leche_que_transporta = highest_type_of_milkd(sol[i]);
            int supply_total = get_supply_per_route(sol[i]);
            if(supply_total > capacidad_camion[i]){
                sobre_exceso += (supply_total -capacidad_camion[i]);
            }
        }

        
        quality = quality -(cuota_faltante*factor_penalizacion + sobre_exceso*factor_sobrecuota);
    }
    return quality;


    
}

double nf(){
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);
    return dist(e2);
}


double alpha;
double initial_temp;

void simulated_aneling(vector<vector<int>> solucion_inicial, int iter){
    vector<vector<int>> best_solution = solucion_inicial;
    double best_quality = evaluar_calidad(best_solution);


    double current_temp = initial_temp;
    int contador = 0;

    vector<vector<int>> current_solution = solucion_inicial;

    vector<vector<int>> vecino;
    
    double calidad_solucion = evaluar_calidad(current_solution);
    double calidad_vecino;

    clock_t begin;
    begin = clock();
    double elapsed_secs;

    while(contador < iter){
        
        vecino = move(current_solution);
        calidad_vecino = evaluar_calidad(vecino);
        double cost_diff = calidad_vecino-calidad_solucion ;

        if(calidad_vecino > best_quality){
            
            best_quality = calidad_vecino;
            best_solution = vecino;
        }

        if (cost_diff > 0){
            current_solution = vecino;
            calidad_solucion = calidad_vecino;
        }else{

            double probabilty = exp(cost_diff/current_temp);
            double random = nf();
            
            if(probabilty > random){
                
                
                current_solution = vecino;
                calidad_solucion = calidad_vecino;
            }
        }
        current_temp = current_temp*alpha;
        contador++;
    }
    clock_t end = clock();
    elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    print_solucion(best_solution,elapsed_secs);
}



int main(int argc, char *argv[]) {
    leer_input(argv[1]);
    int iteraciones = atoi(argv[2]);
    factor_sobrecuota = 0.99;
    factor_penalizacion = 0.99;
    alpha = 0.95;
    initial_temp = 100;
    
    vector<vector<int>> sol = solucion_inicial();
    print_solucion(sol);

    simulated_aneling(sol,iteraciones);

   



    
    
}

