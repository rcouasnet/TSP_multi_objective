#include "tsp.h"

using namespace std;

Tsp::Tsp(const std::string& fi1, const std::string& fi2){
  i1 = new Instance();
  i2 = new Instance();

  i1->tryLoadFile(fi1);
  i2->tryLoadFile(fi2);
}

void Tsp::evaluation(){
  setTotalCost1(i1->calc_distance(i1->get_city(1), i1->get_city(i1->get_nbcities())));
  setTotalCost1(i2->calc_distance(i2->get_city(1), i2->get_city(i2->get_nbcities())));
  for(unsigned int i = 2 ; i < i1->get_nbcities() ; ++i){ 
    int indice= ordre_parcours.at(i);
    int indice_next= ordre_parcours.at(i+1);
    setTotalCost1(getTotalCost1() + i1->calc_distance(i1->get_city(indice), i1->get_city(indice_next)));
    setTotalCost2(getTotalCost2() + i2->calc_distance(i2->get_city(indice), i2->get_city(indice_next)));
  }
  cout << "Evaluation : " << getTotalCost1() << endl;
  cout << "Evaluation : " << getTotalCost2() << endl;
}

