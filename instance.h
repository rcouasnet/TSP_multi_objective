#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "coordinates.h"

class Instance
{
private:
    int nb_cities;
    std::vector<Coordinates>	cities_positions;
   
public:
    Instance();
    Instance(const Instance& other);
    Instance& operator=(const Instance& other);

    /**
     * Impression sur un flux de l'instance
     * @param out
     * @return le flux donné en paramètre avec l'instance 'imprimé'
     */
    std::ostream& print(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const Instance& instance)
    { return instance.print(out); }
    
    bool tryLoadFile(const std::string& fileName);
    
    int get_nbcities() const; 
    
    const Coordinates& get_city(int id) const;
    
    void displayInstance();

};

#endif // INSTANCE_H
