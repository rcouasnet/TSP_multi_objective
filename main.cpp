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
  
  InstanceTsp *t1 = new InstanceTsp(File_A, File_B);
  t1->initSeeds();
  t1->initPath(0);	// Test avec la première graine
  t1->initEvaluation();
  t1->offlineFilter();
//   t1->initSeeds();
//   t1->initPath(0);	// Test avec la première graine
//   t1->initEvaluation();
  
  cout << "< ! Test ! >";
  i1.trySaveToTxt("../data/results/500_krA100.tsp");
  cout << "</ ! Test ! >";

  return 0;
}
