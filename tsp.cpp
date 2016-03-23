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
  for(int i = 2 ; i < i1->get_nbcities() ; ++i){ 
    setTotalCost1(getTotalCost1() + i1->calc_distance(i1->get_city(i), i1->get_city(i+1)));
    setTotalCost2(getTotalCost2() + i2->calc_distance(i2->get_city(i), i2->get_city(i+1)));
  }
  cout << "Evaluation : " << getTotalCost1() << endl;
  cout << "Evaluation : " << getTotalCost2() << endl;

}

