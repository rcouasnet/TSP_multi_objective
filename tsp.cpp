#include "tsp.h"

using namespace std;

Tsp::Tsp(const std::string& fobj1, const std::string& fobj2) :
    total_cost_1(-1), total_cost_2(-1)
{
  obj1 = new Instance();
  obj2 = new Instance();

  obj1->tryLoadFile(fobj1);
  obj2->tryLoadFile(fobj2);
}

void Tsp::initSeeds()
{
    for(int i = 0; i < NB_SEEDS; ++i){ 
	seeds[i] = 0;
    }
}


void Tsp::initPath(unsigned int seed_num)
{
// Les instances doivent avoir le même nombre de villes
    
    // On initialise le chemin : la position dans le chemin est identique
    //	à la position dans l'instance pour l'instant
    path.resize(obj1->get_nbcities() +1);
    
    path[0]= -1;
    for(unsigned i = 1; i <= obj1->get_nbcities(); ++i){
        path[i]= i;
    }

    // On mélange l'ordre du chemin
    assert(seed_num < NB_SEEDS);
    // On ne doit pas mélanger la première case avec les autres ! d'où le ++
    shuffle(++(path.begin()),path.end(),std::default_random_engine(seeds[seed_num]));
    cout<< "La première case doit être = à -1 : "<< path[0]<< endl;
}


void Tsp::initEvaluation(){
    // Initialisation des matrices de distance
    obj1->setMatriceDistance();
    obj2->setMatriceDistance();
    
    // Calcul du cout total de chaque instance : de la première à l'avant dernière ville
    for(unsigned int i = 1 ; i < obj1->get_nbcities() ; ++i) {
	int indice= path[i];
	int indice_next= path[i+1];
	cout << "path["<<i<<"] == "<< path[i] << endl;
	
	total_cost_1 += obj1->get_distance( indice, indice_next);
	total_cost_2 += obj2->get_distance( indice, indice_next);
    }
    
    // Cout de deplacement entre la dernière ville et la première
    set_total_cost1( obj1->get_distance(obj1->get_nbcities(), 1) );
    set_total_cost2( obj2->get_distance(obj2->get_nbcities(), 1) );
    
    cout << "Evaluation instance 1 : " << get_total_cost1() << endl;
    cout << "Evaluation instance 2 : " << get_total_cost2() << endl;
}
