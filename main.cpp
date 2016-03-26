#include <iostream>
#include "instance.h"
#include "tsp.h"


using namespace std;


int main(int argc, char **argv) {
  Instance i1, i2;
  std::string File_A = "../data/kroA100.tsp";
  std::string File_B = "../data/kroB100.tsp";

  if (i1.tryLoadFile(File_A)){
    cout << "lecture réussie : " << i1 << "\n" << endl;
  }
  else {
      cerr<< "Echec lecture : " << i1 << "\n" << endl;
  }
  
  if (i2.tryLoadFile(File_B)){
    cout << "lecture réussie" << i2 << "\n" << endl;
  }
  else {
      cerr<< "Echec lecture : " << i2 << "\n" << endl;
  }
  
  Tsp *t1 = new Tsp(File_A, File_B);
  t1->evaluation();
  
  i1.trySaveToTxt("< ! Test ! >../data/results/500_krA100.tsp< ! Test ! >");

  return 0;
}
