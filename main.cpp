#include <iostream>
#include "instance.h"

using namespace std;


int main(int argc, char **argv) {
  Instance i;
  if (i.tryLoadFile("../data/kroA100.tsp")){
    cout << "lecture réussie" << endl;
    i.displayInstance();
  }
  
  return 0;
}
