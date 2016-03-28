#include <iostream>
#include "objective.h"
#include "instance_tsp.h"


using namespace std;


int main(int argc, char **argv) {
  Objective i1, i2;
  std::string File_A = "../data/kroA100.tsp";
  std::string File_B = "../data/kroB100.tsp";

  if (i1.tryLoadFile(File_A)){
#if DEBUG_CREATION
    clog << "lecture réussie : " << i1 << endl;
#endif
  }
  
  if (i2.tryLoadFile(File_B)){
#if DEBUG_CREATION
    clog << "lecture réussie : " << i2 << endl;
#endif
  }
  
  InstanceTsp *tsp = new InstanceTsp(File_A, File_B);
  tsp->initSeeds();
  tsp->initBothCostMatrices();
  
//  tsp->offlineFilter();
  tsp->onlineFilter();

  
  std::cout << tsp->getInstanceName() << std::endl;  
  
//   tsp->trySaveParetoToTxt(not_dominated"../data/results/nomfic_test_pareto.txt");
  
  return 0;
}
