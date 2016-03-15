#include <iostream>
#include "instance.h"

using namespace std;


int main(int argc, char **argv) {
  Instance i;
  if (i.tryLoadFile("../data/kroA100.tsp")){
    cout << "lecture réussie" << endl;
    cout << i<< endl;
  }
  
  Coordinates A(4, 7);
  Coordinates B(3, 1);

  cout << "< ! Test ! > distance[A(4,7), B(3,1)] = " << i.distance(A, B) << " < ! Test ! >" << endl;
 
  return 0;
}

