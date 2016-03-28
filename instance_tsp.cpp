#include "instance_tsp.h"
#include <boost/concept_check.hpp>
#include <boost/iterator/iterator_concepts.hpp>

using namespace std;

InstanceTsp::InstanceTsp(const std::string& fobj1, const std::string& fobj2) :
   total_cost_1(-1), total_cost_2(-1)
{
  
  File_1 = fobj1;
  File_2 = fobj2;
  
  instance_name = formInstanceName(fobj1, fobj2);
  
  obj1 = new Objective();
  obj2 = new Objective();

  obj1->tryLoadFile(fobj1);
  obj2->tryLoadFile(fobj2);
  
  // @SEE est-ce qu'on laisse kroAB de base ?
  ostringstream oss;
  oss<< "_kroAB"<< obj1->get_nbcities(); 
  instance_name= oss.str();
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
    
#if DEBUG_EVAL
    cout << "Evaluation instance 1 : " << get_total_cost1() << endl;
    cout << "Evaluation instance 2 : " << get_total_cost2() << endl;
#endif
}

bool InstanceTsp::trySaveOfflineParetoToTxt(std::vector< Evaluation* >& notDominated) const
{
    ostringstream oss;
    oss << "../data/results/offlinePareto"<< NB_SEEDS<< instance_name<< ".txt";
    string file_name(oss.str());
    
    return trySaveParetoToTxt(notDominated, file_name);
}

bool InstanceTsp::trySaveOnlineParetoToTxt(std::vector< Evaluation* >& notDominated) const
{
    ostringstream oss;
    oss << "../data/results/onlinePareto"<< NB_SEEDS<< instance_name<< ".txt";
    string file_name(oss.str());
    
    return trySaveParetoToTxt(notDominated, file_name);
}
    
bool InstanceTsp::trySaveParetoToTxt(vector< Evaluation*>& notDominated, const string fileName) const
{
    clog << "Enregistrement dans le fichier "<< fileName<< endl;
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement Pareto500" << endl;
        return false;
    }else{
        for(const Evaluation* eval : notDominated){
            file << eval->get_val1()<< " " << eval->get_val2() << endl;
        }
        file.close();
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

void InstanceTsp::Permutation(Coordinates p){

  int aux = path.at(p.getCol());
  path.at(p.getCol()) = path.at(p.getRow());
  path.at(p.getRow()) = aux;
}

void InstanceTsp::offlineFilter()
{
    double result[500][2] = { { 0 } };
    
    string offline_name = "../data/results/offline500_"+getInstanceName()+".txt";
    ofstream file(offline_name);
 
// Initialisation des chemin et évaluations
    for(unsigned i= 0; i < NB_SEEDS; ++i) {
        generatePath(i);
	  // Evaluation du chemin
	initEvaluation();
	result[i][0] = total_cost_1;
	result[i][1] = total_cost_2;
    }
    
//Sauvegarde de Offline dans le fichier d'enregistrement
    if(!file){
	cerr << "Erreur de création du fichier d'enregistrement" << endl;
    }
    else{
	for(int i=0 ; i < NB_SEEDS ; ++i){
	    file << result[i][0] << " " << result[i][1] << endl;
	}
	file.close();
    }
    
// Recherche du front Pareto
    // Ajout de la solution courante dans le vecteur
    vector<Evaluation*> not_dominated;
    not_dominated.push_back(new Evaluation(result[0][0], result[0][1]));

    for(int k = 1; k < NB_SEEDS ; ++k){
	if ( (k % 5) == 0  ) clog << k<< " solutions évaluées"<< endl;
        unsigned num_eval = 0;
        bool determined = false;

        while(num_eval < not_dominated.size() && !determined) {
	    Evaluation* current_eval= not_dominated[num_eval];
	    
            if( result[k][0] > current_eval->get_val1() 
		    && result[k][1] >current_eval->get_val2() )
	    {
                determined = true;

            }else if( result[k][0] < current_eval->get_val1()
		    && result[k][1] < current_eval->get_val2() ){
		
		clog << "Propagation" << endl;
                current_eval->set_obj1(result[k][0]);
                current_eval->set_obj2(result[k][1]);

		// Propagation
                vector<int> toDelete;
                for(unsigned j= not_dominated.size() -1 ; j > num_eval; --j){
                    if( result[k][0] <  current_eval->get_val1()
			    && result[k][1] < current_eval->get_val2() )
		    {
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
      
    //Sauvegarde de OfflinePareto dans le fichier d'enregistrement
    trySaveOfflineParetoToTxt(not_dominated);
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
    
    string online_name =   "../data/results/online500_"+getInstanceName()+".txt";
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

            // Sauvegarde de la solution courante dans le fichier
            file << total_cost_1<< " " << total_cost_2<< endl;
// 	    TODO Changer : mettre l'évaluation courante dans la classe
	    Evaluation* tsp_evaluation = new Evaluation(total_cost_1, total_cost_2);

            // Test du chemin avec tous les non-dominés
            unsigned indice = 0;
            bool is_determined= false;

	    unsigned size_not_dominated= not_dominated.size();
	    cout << "nombre non dominés : "<< size_not_dominated<< endl;
            while( indice < not_dominated.size() && !is_determined){
		Evaluation* current_eval= not_dominated[num_eval];
		
		// On vérifie que la solution n'est pas dominée
                if( tsp_evaluation->is_dominated(*current_eval) ){
                    is_determined= true;
                }
                // Sinon, on regarde si elle domine
                else if( current_eval->is_dominated(*tsp_evaluation) ){
                    current_eval->set_obj1(total_cost_1);
                    current_eval->set_obj2(total_cost_2);
		    
		    // Propagation
		    spread(not_dominated, tsp_evaluation, indice);

                    is_determined= true;
                }
                // Sinon, c'est qu'aucune des 2 ne domine l'autre, on garde les 2, 
                //	et on vérifie avec la suivante
                else{
                    ++indice;
                }
            }

            if(!is_determined){
                not_dominated.push_back(tsp_evaluation);
            }
            else {
// 		delete &tsp_evalulation;
            }
        }

        // Enregistrement du front Pareto dans un fichier
	trySaveOnlineParetoToTxt(not_dominated);
    }
}


void InstanceTsp::mSTP(unsigned nb_iteration)
{    
    vector<InstanceTsp> not_determined;
    InstanceTsp *active = new InstanceTsp(getFile1(), getFile2());
    InstanceTsp neighboor(getFile1(), getFile2());
    
    string instanceName =  "../data/results/global_pareto_"+active->getInstanceName()+".txt";
    ofstream file(instanceName);

    // Calcul de l'ensemble des permutations possibles pour une instance	
    vector<Coordinates> permut;
    for(unsigned i=0; i < active->getFirstObjective()->get_nbcities(); ++i){
	for(unsigned j= i; j < active->getSecondObjective()->get_nbcities(); ++j){
	    permut.push_back(*(new Coordinates(i, j)));
	}
    }
    
    for(unsigned i = 0 ; i < nb_iteration ; ++ i)
    {
	unsigned nb_evaluation = 0;	
	chrono::high_resolution_clock::time_point debut = chrono::high_resolution_clock::now();	// Timer start
	not_determined.empty();
	
	size_t s = chrono::system_clock::now().time_since_epoch().count();
	
	active->generatePath(s);
	active->initEvaluation();
	
	not_determined.push_back(*active);
	unsigned p = 0;
	
	while(p < not_determined.size() )
	{  
	    *active = not_determined.at(p);	
	    active->initEvaluation();
	    
	    for(int k = 0; k < (int)permut.size(); ++k)
	    {
		neighboor = *active;
		neighboor.Permutation(permut.at(k));
		neighboor.initEvaluation();
		
		unsigned t = 0;
		bool deter = false;
		
		while( t < not_determined.size() && deter == false)
		{
		    ++nb_evaluation;
		    not_determined[t].initEvaluation();
		    
		    if(neighboor.get_total_cost1() > not_determined[t].get_total_cost1() && neighboor.get_total_cost2() > not_determined[t].get_total_cost2())
		    {
			deter = true;
		    }
		    else if(neighboor.get_total_cost1() < not_determined[t].get_total_cost1() && neighboor.get_total_cost2() < not_determined[t].get_total_cost2())
		    { 
			  not_determined.at(t) = neighboor;
			  // Propagation
			  vector<int> toDelete;
			  for(unsigned ind = t+1 ; ind < not_determined.size(); ++ind){
			    
			      if(neighboor.get_total_cost2() <  not_determined[ind].get_total_cost1() && neighboor.get_total_cost2() < not_determined[ind].get_total_cost2()){
				  toDelete.push_back(ind);
			      }
			  }
			  for(unsigned del = 0; del < toDelete.size(); ++del){
			      not_determined.erase(not_determined.begin()+toDelete[del]-del);
			  }

			  deter = true; 		  
		    }else{
			++t;		  
		    }	      
		
		}
		
		if(deter == false){
		    not_determined.push_back(neighboor);
		}
	  
	    }
	    
	    ++p;
	}
	
	chrono::high_resolution_clock::time_point fin = chrono::high_resolution_clock::now();	// Timer fin
		
	auto duration = chrono::duration_cast<chrono::microseconds>(fin - debut).count();

	if(!file){
	    cerr << "Erreur lors de la lecture ou création du fichier" << endl;
	}
	else
	{
	    file << "Nombre d'itération : " << i << endl;
	    file << "Temps d'exécution : " << duration*1000000 << " secondes" << endl;
	    file << "Nombre d'évaluations : "<< nb_evaluation << endl;
	    for(unsigned l=0; l < not_determined.size(); ++l)
	    {
		file << not_determined[l].get_total_cost1() << " " << not_determined[l].get_total_cost2() << endl;
	    }

	    cout << "Ecriture terminé" << endl;
	}     
    }
}