#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

class Instance
{
private:
    int nb_cities;
    
   
public:
    Instance();
    Instance(const Instance& other);
    Instance& operator=(const Instance& other);

    bool tryLoadFile(const std::string& fileName);

};

#endif // INSTANCE_H
