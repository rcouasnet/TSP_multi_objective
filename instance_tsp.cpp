#include "instance_tsp.h"
#include <boost/concept_check.hpp>

using namespace std;

InstanceTsp::InstanceTsp(const std::string& fobj1, const std::string& fobj2) :
   file_i1(fobj1), file_i2(fobj2), total_cost_1(-1), total_cost_2(-1)
{
  
  instanceName = formInstanceName(file_i1, file_i2);
  
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
    
bool InstanceTsp::trySaveParetoToTxt(vector< Evaluation*>& notDominated, const string& fileName) const
{
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement Pareto500" << endl;
        return false;
    }else{
        for(const Evaluation* eval : notDominated){
            file << eval->get_val1()<< " " << eval->get_val2() << endl;
        }
    }
    return true;
}

string InstanceTsp::splitFileName(const string &str)
{
    size_t f = str.find_last_of("/\\");
    string s = str.substr(f+1);
    string fileName = s.substr(0,s.size()-4);

    return fileName;
}

string InstanceTsp::formInstanceName(const string& File_A, const string& File_B)
{
     string fileName_A = splitFileName(File_A);
     string fileName_B = splitFileName(File_B);
     string instanceName = fileName_A.substr(0,4)+fileName_B.substr(3,1)+
                        fileName_A.substr(4,fileName_A.size()-4);
     return instanceName;
}


void InstanceTsp::offlineFilter()
{
    double result[500][2] = { { 0 } };
    
    string offline_name = "../data/results/offline/offline500_"+getInstanceName()+".txt";
    ofstream file(offline_name);
 
     if(!file){
         cerr << "Erreur de création du fichier d'enregistrement" << endl;
     }
     else{
       for(unsigned i= 0; i < 500; ++i) {
	  generatePath(i);
	  // Evaluation du chemin
	  initEvaluation();
	  result[i][0] = total_cost_1;
	  result[i][1] = total_cost_2;
      }

      // Ajout de la solution courante dans le vecteur
      vector<Evaluation*> not_dominated;
      not_dominated.push_back(new Evaluation(result[0][0], result[0][1]));

      for(int k = 1; k < 500 ; ++k){
	  unsigned num_eval = 0;
	  bool determined = false;

	  while(num_eval < not_dominated.size() && !determined){
	      Evaluation* current_eval= not_dominated.at(num_eval);
	      
	      if( result[k][0] > current_eval->get_val1() 
		      && result[k][1] >current_eval->get_val2() ) {

		  determined = true;
	      
	      }else if( result[k][0] < current_eval->get_val1()
			  && result[k][1] < current_eval->get_val2() ){
		  current_eval->set_obj1(result[k][0]);
		  current_eval->set_obj2(result[k][1]);
                  // Propagation
		  vector<int> toDelete;
		  for(unsigned j = num_eval + 1 ; j < not_dominated.size(); ++j) {
		      if( result[k][0] <  current_eval->get_val1()
			      && result[k][1] < current_eval->get_val2() ){
			  not_dominated.erase(not_dominated.begin() + j);
		      }
		  }
		  
		  determined = true;

	      }else{
		  ++num_eval;
	      }
	  }

	  if(determined == false){
	      not_dominated.push_back(new Evaluation(result[0][0], result[0][1]));
	  }
      }
      
      for(int i=0 ; i < 500 ; ++i){
	  file << result[i][0] << " " << result[i][1] << endl;
      }

 
     //Sauvegarde de OfflinePareto500 dans le fichier d'enregistrement
     string paretoFileName = "../data/results/offline/offlinePareto500_"+getInstanceName()+".txt";
     trySaveParetoToTxt(not_dominated, paretoFileName);
  }
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
    
    string online_name =   "../data/results/online/online500_"+getInstanceName()+".txt";
    ofstream file(online_name);

    if(!file){
        cerr << "Erreur de création du fichier d'enregistrement" << endl;
    }
    else{
	// Test des 500 solutions
        for(unsigned num_eval= 0; num_eval < 500 ; ++num_eval){
	    
            generatePath(num_eval);
            // Evaluation du chemin
            initEvaluation();

            // Sauvegarde de la solution courante
            file << total_cost_1<< " " << total_cost_2<< endl;
// 	    TODO Changer : mettre l'évaluation courante dans la classe
	    Evaluation* tsp_evaluation = new Evaluation(total_cost_1, total_cost_2);

            // Test du chemin avec tous les non-dominés
            unsigned indice = 0;
            bool is_dominating= false;

            while( indice < not_dominated.size() && !is_dominating){
		Evaluation* current_eval= not_dominated[num_eval];
		
		// On vérifie que la solution n'est pas dominée
                if( tsp_evaluation->is_dominated(*current_eval) ){
                    is_dominating= true;
                }
                // Sinon, on regarde si elle domine
                else if( current_eval->is_dominated(*tsp_evaluation) ){
                    current_eval->set_obj1(total_cost_1);
                    current_eval->set_obj2(total_cost_2);
		    
		    // Propagation
		    spread(not_dominated, tsp_evaluation, indice);

                    is_dominating= true;
                }
                else{
                    ++indice;
                }
            }

            if(!is_dominating){
                not_dominated.push_back(tsp_evaluation);
            }
            else {
		delete &tsp_evaluation;
            }
        }

	//Sauvegarde de OnlinePareto500 dans le fichier d'enregistrement
        string paretoFileName = "../data/results/online/onlinePareto500_"+getInstanceName()+".txt";
	trySaveParetoToTxt(not_dominated, paretoFileName);
    }
}

