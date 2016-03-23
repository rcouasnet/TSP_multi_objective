#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "instance.h"
#include "coordinates.h"

class Tsp
{
private:
    Instance *i1, *i2;
    std::string InstanceFile1, InstanceFile2;
    double total_cost_1, total_cost_2;
    
public:
      Tsp(const std::string& if1, const std::string& if2);
      void evaluation();
      double getTotalCost1(){	return total_cost_1;  }
      double getTotalCost2(){	return total_cost_2;  }
      void setTotalCost1(double cost){	total_cost_1 = cost;  }
      void setTotalCost2(double cost){	total_cost_2 = cost;  }
  
};

#endif // TSP_INSTANCE_H