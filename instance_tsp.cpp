#include "instance_tsp.h"
#include <boost/concept_check.hpp>

using namespace std;

InstanceTsp::InstanceTsp(const std::string& fobj1, const std::string& fobj2) :
    total_cost_1(-1), total_cost_2(-1)
{
  obj1 = new Objective();
  obj2 = new Objective();

  obj1->tryLoadFile(fobj1);
  obj2->tryLoadFile(fobj2);
}

void InstanceTsp::initSeeds()
{
    // obtain a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    seeds[0]= seed;
    
    for(unsigned i = 1; i < NB_SEEDS; ++i){ 
	seeds[i] = i;
    }
}

void InstanceTsp::initBothCostMatrices()
{
    // Initialisation des matrices de distance
    obj1->setMatriceDistance();
    obj2->setMatriceDistance();
}
    
void InstanceTsp::generatePath(unsigned seed_num)
{
// Les instances doivent avoir le même nombre de villes
    
    // On initialise le chemin : la position dans le chemin est identique
    //	à la position dans l'instance pour l'instant
    path.resize(obj1->get_nbcities() +1);
    
    path[0]= -1;
    for(unsigned i = 1; i <= obj1->get_nbcities(); ++i){
        path[i]= i;
    }
    
    assert(seed_num < NB_SEEDS);
    
    // On mélange l'ordre du chemin
    // On ne doit pas mélanger la première case avec les autres ! d'où le ++
    shuffle(++(path.begin()),path.end(),std::default_random_engine(seeds[seed_num]));
    
    assert(path[0] == -1);
}


void InstanceTsp::initEvaluation()
{        
    // Cout de deplacement entre la dernière ville et la première
    set_total_cost1( obj1->get_distance(obj1->get_nbcities(), 1) );
    set_total_cost2( obj2->get_distance(obj2->get_nbcities(), 1) );
    
    // Calcul du cout total de chaque instance : de la première à l'avant dernière ville
    for(unsigned int i = 1 ; i < obj1->get_nbcities() ; ++i) {
	int indice= path[i];
	int indice_next= path[i+1];
#if DEBUG_EVAL
	cout << "path["<<i<<"] == "<< path[i] << endl;
#endif
	
	total_cost_1 += obj1->get_distance( indice, indice_next);
	total_cost_2 += obj2->get_distance( indice, indice_next);
    }
    
    cout << "Evaluation instance 1 : " << get_total_cost1() << endl;
    cout << "Evaluation instance 2 : " << get_total_cost2() << endl;
}
    
bool InstanceTsp::trySaveParetoToTxt(vector<Coordinates> not_dominated, const string& fileName)
{
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement" << endl;
        return false;
    }else{
        for(unsigned i=0; i < not_dominated.size(); ++i){
            file << not_dominated[i].col << " " << not_dominated[i].row << endl;
        }
    }
    return true;
}



void InstanceTsp::offlineFilter()
{
    double result[500][2] = { { 0 } };

    for(unsigned i= 0; i < 500; ++i) {
        generatePath(i);
	initEvaluation();
	result[i][0] = total_cost_1;
	result[i][1] = total_cost_2;
    }

    vector<Coordinates> not_dominated;
    not_dominated.push_back(*(new Coordinates(result[0][0], result[0][1])));

    for(int k = 1; k < 500 ; ++k){
        unsigned l = 0;
        bool determined = false;

        while(l < not_dominated.size() && determined == false){
            if( (result[k][0] > not_dominated.at(l).col  && result[k][1] >not_dominated.at(l).row )) {

                determined = true;

            }else if( result[k][0] < not_dominated.at(l).col  && result[k][1] < not_dominated.at(l).row ){
                not_dominated.at(l).col= result[k][0];
                not_dominated.at(l).row= result[k][1];

                vector<int> toDelete;
                for(unsigned ind = l+1 ; ind < not_dominated.size(); ++ind){
                    if( (result[k][0] <  not_dominated.at(ind).col && result[k][1] < not_dominated.at(ind).row) ){
                        toDelete.push_back(ind);
                    }
                }

                for(unsigned del=0; del < toDelete.size(); ++del){
                    not_dominated.erase(not_dominated.begin()+toDelete.at(del)-del);
                }
                determined = true;

            }else{
                ++l;
            }
        }

        if(determined == false){
            not_dominated.push_back(*(new Coordinates(result[0][0], result[0][1])));
        }
    }
    
    
    //offline500
     string instName =   "../data/results/offline500_test.txt";
     ofstream file1(instName);
 
     if(!file1){
         cerr << "Erreur pendant l'ouverture du fichier d'enregistrement" << endl;
     }
     else{
         for(int i=0 ; i < 500 ; ++i){
 
            file1 << result[i][0] << " " << result[i][1] << endl;
         }
     }
 
     //offline500 Pareto
     trySaveParetoToTxt(not_dominated, "../data/results/offline500_Pareto_test.txt");
}


void InstanceTsp::spread(vector< Evaluation* >& not_dominated, Evaluation* eval, unsigned ind_begin)
{
    for(unsigned j = not_dominated.size() -1 ; j > ind_begin; ++j)
    {
	if( not_dominated[j]->is_dominated(*eval) ) {
	    #if DEBUG_ONLINE
		cout << "Suppression de l'élement à la place "<< j<< " des non dominés"<< endl;
	    #endif
	    not_dominated.erase(not_dominated.begin() + j);
	}
    }

}

void InstanceTsp::onlineFilter()
{
    vector< Evaluation* > not_dominated;
    string instanceName= "TEMPORAIRE"; // TODO mettre dans la classe

    string online_name =   "results/online/online500_"+instanceName+".txt";
    ofstream file(online_name);

    if(!file){
        cerr << "Erreur de création du fichier" << endl;
    }
    else{
	// Test des 500 solutions
        for(unsigned num_sol= 0; num_sol < 500 ; ++num_sol){
	    
            generatePath(num_sol);
            // Evaluating the path
            initEvaluation();

            // Writing current solution in file online500 ...
            file << total_cost_1<< " " << total_cost_2<< endl;
// 	    TODO Changer : mettre l'évaluation courante dans la classe
	    Evaluation* tsp_evalulation= new Evaluation(total_cost_1, total_cost_2);

            // Testing the path with all the non-dominated ones
            unsigned indice = 0;
            bool is_dominating= false;

            while( indice < not_dominated.size() && !is_dominating){
		Evaluation* current_eval= not_dominated[num_sol];
		
		// On vérifie que la solution n'est pas dominée
                if( tsp_evalulation->is_dominated(*current_eval) ){
                    is_dominating= true;
                }
                // Sinon, on regarde si elle domine
                else if( current_eval->is_dominated(*tsp_evalulation) ){
                    current_eval->set_obj1(total_cost_1);
                    current_eval->set_obj2(total_cost_2);
		    
		    // Propagation
		    spread(not_dominated, tsp_evalulation, indice);

                    is_dominating= true;
                }
                else{
                    ++indice;
                }
            }

            if(!is_dominating){
                not_dominated.push_back(tsp_evalulation);
            }
            else {
		delete &tsp_evalulation;
            }
        }

        // Writing Pareto local front in onlinePareto500_instanceName.txt
        string paretoFileName = "results/online/onlinePareto500_"+instanceName+".txt";

        ofstream fileP(paretoFileName);

        if(!fileP){
            cerr << "Erreur de création de fichier" << endl;
        }else{
            for(unsigned i=0; i < not_dominated.size(); ++i){
                fileP << not_dominated[i]->get_val1() << " " << not_dominated[i]->get_val2() << endl;
            }
        }
    }
}

