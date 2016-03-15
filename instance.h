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
    unsigned int nb_cities;
    std::vector<Coordinates>	cities_positions;
   
public:
    Instance();
    Instance(const Instance& other);
    Instance& operator=(const Instance& other);

    int get_nbcities() const; 
    
    const Coordinates& get_city(unsigned int id) const;
    
    /**
     * Impression sur un flux de l'instance
     * @param out
     * @return le flux donné en paramètre avec l'instance 'imprimé'
     */
    std::ostream& print(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const Instance& instance)
    { return instance.print(out); }
    
    /**
     * Charge le fichier d'instance et initialise l'instance
     * @param fileName chemin du fichier chargé
     * @return true si le fichier est correctement chargé
     */
    bool tryLoadFile(const std::string& fileName);
    /**
     * Calcule la distance entre 2 coordonnées
     * @param A première coordonnée
     * @param B seconde coordonnée
     */
    float calc_distance(Coordinates A, Coordinates B);
    /**
     * Calcule la distance entre 2 villes
     * @param A première ville
     * @param B seconde ville
     */
    float calc_distance(int A, int B);
    
    std::vector< std::vector<float> >& matriceDistance(Instance inst);
    
};

#endif // INSTANCE_H
