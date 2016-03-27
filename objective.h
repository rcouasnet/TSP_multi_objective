#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <ostream>

#include "debug.h"

#include "coordinates.h"

class Objective
{
private:
    unsigned int nb_cities;
    std::vector<Coordinates>	cities_positions;
    std::vector< std::vector <float> > cost_matrice;
   
public:
    Objective();
    Objective(const Objective& other);
    Objective& operator=(const Objective& other);

    /** Getters **/
    unsigned get_nbcities() const {  return nb_cities; } 

    const Coordinates& get_city(unsigned int id) const;
    
    /** Setters	**/
    /**
     * Initialise les tailles de la matrice de distance
     */
    void initCostMatrice();
    
    /**
     * Impression sur un flux de l'instance
     * @param out
     * @return le flux donné en paramètre avec l'instance 'imprimé'
     */
    std::ostream& print(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const Objective& instance)
    { return instance.print(out); }
    
    /**
     * Charge le fichier d'instance et initialise l'instance
     * @param fileName chemin du fichier chargé
     * @return true si le fichier est correctement chargé
     */
    bool tryLoadFile(const std::string& fileName);
    
/***	Operations sur les distances	***/
private:
    /**
     * Calcule la distance entre 2 coordonnées
     * @param A première coordonnée
     * @param B seconde coordonnée
     */
    double calcDistance(const Coordinates& A, const Coordinates& B);
    /**
     * Calcule la distance entre 2 villes
     * @param A première ville
     * @param B seconde ville
     */
    double calcDistance(double ind_A, double ind_B);

public:
    /**
     * @return la matrice de distance de l'instance
     */
    void setMatriceDistance();
    /**
     * Calcule la distance entre 2 villes
     * @param A première ville
     * @param B seconde ville
     */
    float get_distance(double ind_A, double ind_B) const; 
};

std::vector<std::string>& explode(const std::string &str);

#endif // INSTANCE_H
