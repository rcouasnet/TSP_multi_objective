#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <ostream>

#include "coordinates.h"

class Instance
{
private:
    unsigned int nb_cities;
    std::vector<Coordinates>	cities_positions;
    std::vector< std::vector <double> > cost_matrice;
   
public:
    Instance();
    Instance(const Instance& other);
    Instance& operator=(const Instance& other);

    unsigned get_nbcities() const; 
    
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
    float calc_distance(const Coordinates& A, const Coordinates& B);
    /**
     * Calcule la distance entre 2 villes
     * @param A première ville
     * @param B seconde ville
     */
    float calc_distance(int A, int B);
    /**
     * @return la matrice de distance de l'instance
     */
    std::vector< std::vector<float> >& matriceDistance();
    
    /** Sauvegarde pour GNU Plot **/
    /**
     * Sauvegarde de l'instance dans un fichier texte
     * @param fileName : chemin du fichier
     * @return false si le fichier ne peut pas être crée
     */
    bool trySaveToTxt(const std::string& fileName);
    
    /**
     * Sauvegarde du front Pareto
     * @param fileName : chemin du fichier
     * @return false si le fichier ne peut pas être crée
     * TODO
     */
    bool trySaveParetoToTxt(const std::string& fileName);
    
};

std::vector<std::string>& explode(const std::string &str);

#endif // INSTANCE_H
