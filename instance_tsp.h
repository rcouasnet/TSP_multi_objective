#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <assert.h>

#include "debug.h"

#include "objective.h"
#include "coordinates.h"

#define NB_SEEDS	500

class InstanceTsp
{
private:
    Objective *obj1, *obj2;
    std::string file_i1, file_i2;
    double total_cost_1, total_cost_2;
    
    std::vector<int> path;
    
    unsigned seeds[NB_SEEDS];
    
public:
    InstanceTsp(const std::string& fi1, const std::string& fi2);
    /*** Initialisations ***/
    /**
     * Initialise les 500 graines aléatoires
     * TODO pour l'instant, elles sont initialisée à 0, à changer
     */
    void initSeeds();
    /**
     * Crée un parcours aléatoire à partir d'une graine aléatoire
     * @param random_seed graine aléatoire utilisée
     */
    void initPath(unsigned seed_num);
    /**
     * Calcule le cout total de chaque objectif
     * @SEE voir si le calcul des matrices de distance se fait en dehors de la méthode
     */
    void initEvaluation();
    
    /*** Getters ***/
    double get_total_cost1(){	return total_cost_1;  }
    double get_total_cost2(){	return total_cost_2;  }
    /** Setters ***/
    void set_total_cost1(double cost){	total_cost_1 = cost;  }
    void set_total_cost2(double cost){	total_cost_2 = cost;  }
    
       /**
     * Sauvegarde du front Pareto
     * @param fileName : chemin du fichier
     * @return false si le fichier ne peut pas être crée
     * TODO
     */
    bool trySaveParetoToTxt(std::vector<Coordinates> notDominated, const std::string& fileName);
    
    void offlineFilter();
  
};

#endif // TSP_INSTANCE_H