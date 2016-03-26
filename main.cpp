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
  
<<<<<<< HEAD
  Coordinates A(4, 7);
  Coordinates B(3, 1);

  cout << "< ! Test ! > distance[A(4,7), B(3,1)] = " << i.calc_distance(A, B) << " < ! Test ! >" << endl;
  
  i.trySaveToTxt("../data/results/500_krA100.tsp");
=======
  if (i2.tryLoadFile(File_B)){
    cout << "lecture réussie" << i1 << "\n" << endl;
  }
  else {
      cerr<< "Echec lecture : " << i2 << "\n" << endl;
  }
  
  Tsp *t1 = new Tsp(File_A, File_B);
  t1->evaluation();
>>>>>>> 471d6cdd2c4bc931afcb2564aa551a43b095189a
 

  return 0;
}
