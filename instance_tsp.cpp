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

  if (obj1->tryLoadFile(fobj1) &&  obj2->tryLoadFile(fobj2) ){
      cout << "Fichiers de l'instance correctement ouverts"<< endl;
  }
  else {
      cerr<< "Impossible d'ouvrir les fichiers de l'instance"<< endl;
      exit(EXIT_FAILURE);
  }
  
  // @SEE est-ce qu'on laisse kroAB de base ?
  ostringstream oss;
  oss<< "_kroAB"<< obj1->get_nbcities(); 
  instance_name= oss.str();
}

void InstanceTsp::initSeeds()
{
    // Graine aléatoire random (on pourrait mettre i pour avoir tout le temps les même solutions)
    
    for(unsigned i = 0; i < NB_SEEDS; ++i){ 
	seeds[i]= std::chrono::system_clock::now().time_since_epoch().count();
// 	seeds[i]= i;
    }
}

void InstanceTsp::setBothCostMatrices()
{
    // Initialisation des matrices de distance
    obj1->setMatriceDistance();
    obj2->setMatriceDistance();
}
 
void InstanceTsp::initPath()
{
    // Les instances doivent avoir le même nombre de villes
    
    // On initialise le chemin : la position dans le chemin est identique
    //	à la position dans l'instance pour l'instant
    path.resize(obj1->get_nbcities() +1);
    
    path[0]= -1;
    for(unsigned i = 1; i <= obj1->get_nbcities(); ++i){
        path[i]= i;
    }
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
    
    // On mélange l'ordre du chemin avec un graine déjà crée
    // On ne doit pas mélanger la première case avec les autres ! d'où le ++
    shuffle(++(path.begin()),path.end(),std::default_random_engine(seeds[seed_num]));

#if DEBUG_PATH
    for(unsigned i= 1; i <= obj1->get_nbcities(); ++i) 
    {
	clog << "path["<< i <<"] : "<< path[i] << endl;
	assert(path[i]!= -1);
    }
    clog << endl;
#endif
    assert(path[0] == -1);
}

void InstanceTsp::generatePath()
{
    initPath();
    clog << "je ne dois être utilisé que dans le mTSP"<< endl;
    
    size_t random_seed = chrono::system_clock::now().time_since_epoch().count();
    
    // On mélange l'ordre du chemin
    // On ne doit pas mélanger la première case avec les autres ! d'où le ++
    shuffle(++(path.begin()),path.end(),std::default_random_engine(random_seed));

#if DEBUG_PATH
    for(unsigned i= 1; i <= obj1->get_nbcities(); ++i) 
    {
	clog << "path["<< i <<"] : "<< path[i] << endl;
	assert(path[i]!= -1);
    }
    clog << endl;
#endif
    
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

bool InstanceTsp::trySavemTSP(vector<InstanceTsp>& instances, long duration, int nbIterations, int nbEvaluations)
{
    ostringstream oss;
    oss << "../data/results/global_pareto_"<< nbIterations<< "_"<< instances[0].getInstanceName() +".txt";
    string instanceName = oss.str();
    ofstream file(instanceName);

    if(!file){
	cerr << "Erreur lors de la création du fichier de sauvegarde" << endl;
	return true;
    }
    else
    {
	file << "Nombre d'itération : " << nbIterations << endl;
	file << "Temps d'exécution : " << duration*1000000 << " secondes" << endl;
	file << "Nombre d'évaluations : "<< nbEvaluations << endl;
	for(unsigned l=0; l < instances.size(); ++l)
	{
	    file << instances[l].get_total_cost1() << " " << instances[l].get_total_cost2() << endl;
	}

	cout << "Ecriture terminé" << endl;
	
	return true;
    }
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
		
                current_eval->set_obj1(result[k][0]);
                current_eval->set_obj2(result[k][1]);

		clog << "Propagation" << endl;
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


int InstanceTsp::spread(vector< Evaluation* >& not_dominated, Evaluation* eval, unsigned ind_begin)
{
    int nb_deleted= 0;
    #if DEBUG_ONLINE
	cout << "Propagation de la solution dominante"<< endl;
    #endif
    for(unsigned j = not_dominated.size() -1 ; j > ind_begin; --j)
    {
	if( not_dominated[j]->is_dominated(*eval) ) {
	    ++nb_deleted;
	    #if DEBUG_ONLINE
		cout << "Suppression de l'élement à la place "<< j<< " des non dominés"<< endl;
	    #endif
	    not_dominated.erase(not_dominated.begin() + j);
	}
    }
    
    return nb_deleted;
}

void InstanceTsp::onlineFilter()
{
    vector< Evaluation* > not_dominated;
    Evaluation* tsp_evaluation= NULL;
    
    string online_name =   "../data/results/online500_"+getInstanceName()+".txt";
    ofstream file(online_name);

    if(!file){
        cerr << "Erreur de création du fichier d'enregistrement" << endl;
    }
    else{
	// Test de toutes les solutions
        for(unsigned num_sol= 0; num_sol < NB_SEEDS ; ++num_sol){
	    
            generatePath(num_sol);
            // Evaluation du chemin
            initEvaluation();

            // Sauvegarde de la solution courante dans le fichier
            file << total_cost_1<< " " << total_cost_2<< endl;

	    tsp_evaluation= new Evaluation(total_cost_1, total_cost_2);
	    
	    cout << "Eval tsp : "<< tsp_evaluation->get_val1()<< " ; "<< tsp_evaluation->get_val2()<< endl;

	    clog<< "nombre non dominées : "<< not_dominated.size()<< endl;
	    
            // Test du chemin avec tous les non-dominés
	    bool is_dominated= false; // Si la solution domine une autre, on s'arrête
	    bool is_dominating= false; // Si la solution ou est dominée, on s'arrête
            unsigned ind_nondominated = 0;
            while( ind_nondominated < not_dominated.size() && !is_dominated && !is_dominating){
		Evaluation* current_eval= not_dominated[ind_nondominated];
		
		// On vérifie que la solution n'est pas dominée
                if( tsp_evaluation->is_dominated(*current_eval) ){
		    is_dominated= true;
		    #if DEBUG_ONLINE
			cout << "La solution est dominée par une autre évaluation"<< endl;
		    #endif
		}
                // Sinon, on regarde si elle domine
                else if( current_eval->is_dominated(*tsp_evaluation) ){
		    // C'est le cas : Propagation
		    is_dominating= true;
		    #if DEBUG_ONLINE
			cout << "La solution domine une autre évaluation, propagation"<< endl;
		    #endif
		    ind_nondominated -= spread(not_dominated, tsp_evaluation, ind_nondominated -1);
                }
                // Sinon, c'est qu'aucune des 2 ne domine l'autre, on garde les 2, 
                //	et on vérifie avec la non-dominée suivante
                else{
                    ++ind_nondominated;
		    #if DEBUG_ONLINE
			cout << "Aucune des 2 ne domine, on passe à la suite"<< endl;
		    #endif
                }
            }
            // Si la solution n'est dominée par aucune autre, on l'ajoute
            if (!is_dominated) {
		not_dominated.push_back(tsp_evaluation);
		#if DEBUG_ONLINE
		    cout << "La solution n'est dominée par aucune autre évaluation,"
		    "ajout dans les non dominées"<< endl<< endl;
		#endif
	    }
// 	    else { delete tsp_evaluation; }
        }

        // Enregistrement du front Pareto dans un fichier
	trySaveOnlineParetoToTxt(not_dominated);
    }
}

/***
continue <- vrai
init (A) // aléatoirement

Tant_que continue faire :
	choisir x € A
	choisir x' € N(x) // N(x) : voisin, ... si tout N(x) est exploré, marquer x
	A <- A Union {x'}
	filtrer A (on line)
	|  si toute solution de A est marquée, alors
	|  continuer <- faux

fin_Tantque
retourner A
*/

// Voisinage : échanger 2 valeur et inverser l'ordre des valeurs entre les 2
	
	

void InstanceTsp::mTSP(unsigned nb_iteration)
{    
    vector<InstanceTsp> not_determined;
    InstanceTsp *active = new InstanceTsp(getFile1(), getFile2());
    InstanceTsp neighboor(getFile1(), getFile2());

    // Calcul de l'ensemble des permutations possibles pour une instance	
    vector<Coordinates> permut;
    for(unsigned i=0; i < active->getFirstObjective()->get_nbcities(); ++i){
	for(unsigned j= i; j < active->getSecondObjective()->get_nbcities(); ++j){
	    permut.push_back(*(new Coordinates(i, j)));
	}
    }
    
    // On effectue le nombre d'itérations demandés
    for(unsigned i = 0 ; i < nb_iteration ; ++ i)
    {
	unsigned nb_evaluation = 0;	
	chrono::high_resolution_clock::time_point debut = chrono::high_resolution_clock::now();	// Timer start
// 	not_determined.empty();
	not_determined.clear();
	
	active->generatePath();
	active->initEvaluation();
	
	not_determined.push_back(*active);
	unsigned num_instance = 0;
	
	while(num_instance < not_determined.size() )
	{  
	    *active = not_determined[num_instance];	
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
	    
	    ++num_instance;
	}
	
	chrono::high_resolution_clock::time_point fin = chrono::high_resolution_clock::now();	// Timer fin
	long duration = chrono::duration_cast<chrono::microseconds>(fin - debut).count();

	trySavemTSP(not_determined, duration, nb_evaluation, nb_iteration);
    }
}
