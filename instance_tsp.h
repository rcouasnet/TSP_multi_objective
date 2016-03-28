#ifndef TSP_H
#define TSP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <assert.h>
// #include <ctime>
#include <random>
#include <chrono>
#include <list>

#include "debug.h"

#include "objective.h"
#include "evaluation.h"
#include "coordinates.h"

#define NB_SEEDS	500
// #define NDEBUG  // pour enlever les assert()


class InstanceTsp
{
private:
    Objective *obj1, *obj2;
    std::string File_1, File_2;
    std::string instance_name;
    double total_cost_1, total_cost_2;
    
    std::vector<int> path;
    
    unsigned seeds[NB_SEEDS];
    
public:
    InstanceTsp(const std::string& fi1, const std::string& fi2);
    /*** Initialisations ***/
    /**
     * Initialise les 500 graines aléatoires
     * TODO pour l'instant, elles sont initialisée à 0 (sauf la première), à changer,
     * dans le sujet : "assurez-vous d'utiliser les mêmes graines aléatoires pour vos tests.
     * Ainsi les fronts obtenus obtenus doivent être identiques"
     */
    void initSeeds();
    /**
     * Initialise les 2 matrices de distance
     */
    void setBothCostMatrices();
        
private:
    /**
     * Initialise le chemin: de 1 à nombre-de-villes
     */
    void initPath();
    /**
     * Crée un parcours aléatoire à partir d'une graine aléatoire du vecteur de graines
     * @param seed_num graine aléatoire utilisée
     */
    void generatePath(unsigned seed_num);
    /**
     * Crée un parcours aléatoire à partir d'une graine aléatoire en générée
     * 	(à partir de la "date 0")
     */
    void generatePath();
    /**
     * Calcule le cout total de chaque objectif
     * @SEE voir si le calcul des matrices de distance se fait en dehors de la méthode
     */
    void initEvaluation();
    
/*** Getters ***/

public:
    Objective* getFirstObjective(){ return obj1; }
    Objective* getSecondObjective(){ return obj2; }
    double get_total_cost1(){	return total_cost_1;  }
    double get_total_cost2(){	return total_cost_2;  }
    std::string getFile1(){	return File_1;  }
    std::string getFile2(){	return File_2;  }
    std::string getInstanceName(){	return instance_name;  }
    
/** Setters ***/
    void set_total_cost1(double cost){	total_cost_1 = cost;  }
    void set_total_cost2(double cost){	total_cost_2 = cost;  }
    
/*** Sauvegardes ***/

private:
    /**
     * Sauvegarde du front Pareto
     * @param fileName : chemin du fichier
     * @return false si le fichier ne peut pas être crée
     */
    bool trySaveParetoToTxt(std::vector< Evaluation* >& notDominated, const std::string fileName) const;
    
public:
    /**
     * Sauvegarde du front Pareto offline
     * @return false si le fichier ne peut pas être crée
     */
    bool trySaveOfflineParetoToTxt(std::vector< Evaluation* >& notDominated) const;
    
    /**
     * Sauvegarde du front Pareto
     * @return false si le fichier ne peut pas être crée
     */
    bool trySaveOnlineParetoToTxt(std::vector< Evaluation* >& notDominated) const;
    
    bool trySavemTSP(std::vector< InstanceTsp >& instances, long int duration, int nb_iterations, int nbEvaluations);
    
/*** Fonctions utiles ***/
     /**
     * Découpe la chaîne du chemin du fichier pour retourner le nom du fichier seulement
     * @param str : chemin du fichier
     * @return nom du fichier 
     */
    std::string splitFileName(const std::string &str);
    
     /**
     * Créer le nom de l'instance résultat pour la création du fichier d'enregistrement
     * @param str : chemin du fichier
     * @return nom du fichier 
     */
    std::string formInstanceName(const std::string &File_A, const std::string &File_B);
    
    void Permutation(int p1, int p2);
    
/*** Filtrages ***/
     /**
     * Filtre offline
     */
    void offlineFilter();
    
    /**
     * Propage la domination d'une évaluation sur le reste des non dominées :
     * 	vérifie toutes les valeurs suivantes et les supprime si elles sont également dominées
     * @param not_dominated les solutions sur lesquelles effectuer la propagation
     * @param eval évaluation qui domine peut être les autres
     * @param ind_begin emplacement du vecteur à partir duquel effectuer la propagation
     */
    int spread(std::vector< Evaluation* >& not_dominated, Evaluation* eval, unsigned int ind_begin);
    /**
     * Filtre online
     */
    void onlineFilter();
    
     /**
     * Algorithme mTSP
     */
    void mTSP(unsigned nb_iteration);
  
};

#endif // TSP_INSTANCE_H