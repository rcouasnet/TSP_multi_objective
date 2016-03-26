#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "debug.h"

#include "instance.h"
#include "coordinates.h"

class Tsp
{
private:
    Instance *i1, *i2;
    std::string file_i1, file_i2;
    double total_cost_1, total_cost_2;
    std::vector<int> ordre_parcours;
    
public:
      Tsp(const std::string& fi1, const std::string& fi2);
      void evaluation();
      double getTotalCost1(){	return total_cost_1;  }
      double getTotalCost2(){	return total_cost_2;  }
      void setTotalCost1(double cost){	total_cost_1 = cost;  }
      void setTotalCost2(double cost){	total_cost_2 = cost;  }
  
};

#endif // TSP_INSTANCE_H