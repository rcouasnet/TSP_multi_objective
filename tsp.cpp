#include "tsp.h"

using namespace std;

Tsp::Tsp(const std::string& fi1, const std::string& fi2) :
    total_cost_1(-1), total_cost_2(-1)
{
  i1 = new Instance();
  i2 = new Instance();

  i1->tryLoadFile(fi1);
  i2->tryLoadFile(fi2);
}

void Tsp::evaluation(){
    // Initialisation des matrices de distance
    i1->setMatriceDistance();
    i2->setMatriceDistance();
    
    // Cout de deplacement entre la première et la dernière ville
    setTotalCost1( i1->get_distance(1, i1->get_nbcities()) );
    setTotalCost1( i2->get_distance(1, i2->get_nbcities()) );
    
    // Calcul du cout total de chaque instance
    for(unsigned int i = 2 ; i < i1->get_nbcities() ; ++i){ 
// 	@SEE l'ordre parcours est-il utile ici ?
// 	int indice= ordre_parcours.at(i);
// 	int indice_next= ordre_parcours.at(i+1);
	
	int indice= i;
	int indice_next= i+1;
	
	total_cost_1 += i1->get_distance( indice, indice_next);
	total_cost_2 += i2->get_distance( indice, indice_next);
    }
    cout << "Evaluation instance 1 : " << getTotalCost1() << endl;
    cout << "Evaluation instance 2 : " << getTotalCost2() << endl;
}
