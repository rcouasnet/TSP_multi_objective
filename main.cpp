#include <iostream>
#include "instance.h"
#include "tsp.h"


using namespace std;


int main(int argc, char **argv) {
  Instance i1, i2;
  std::string File_A = "../data/kroA100.tsp";
  std::string File_B = "../data/kroB100.tsp";

  if (i1.tryLoadFile(File_A)){
    clog << "lecture réussie : " << i1 << "\n" << endl;
  }
  
  if (i2.tryLoadFile(File_B)){
    clog << "lecture réussie" << i2 << "\n" << endl;
  }
  
  Tsp *t1 = new Tsp(File_A, File_B);
  t1->initSeeds();
  t1->initPath(0);	// Test avec la première graine
  t1->initEvaluation();
  
  cout << "< ! Test ! >";
  i1.trySaveToTxt("../data/results/500_krA100.tsp");
  cout << "</ ! Test ! >";

  return 0;
}
