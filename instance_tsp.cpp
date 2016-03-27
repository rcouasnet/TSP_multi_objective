#include "instance_tsp.h"

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

void InstanceTsp::initPath(unsigned int seed_num)
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


void InstanceTsp::initEvaluation(){
    // Initialisation des matrices de distance
    obj1->setMatriceDistance();
    obj2->setMatriceDistance();
        
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


bool InstanceTsp::trySaveParetoToTxt(vector<Coordinates> notDominated, const string& fileName)
{
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement" << endl;
        return false;
    }else{
        for(unsigned i=0; i < notDominated.size(); ++i){
            file << notDominated.at(i).getCol() << " " << notDominated.at(i).getRow() << endl;
        }
    }
    return true;
}



void InstanceTsp::offlineFilter()
{
    double result[500][2] = { { 0 } };

    vector<int> p;

    for(int i=0; i < 500; ++i){
        initPath(seeds[i]);
	initEvaluation();
	result[i][0] = total_cost_1;
	result[i][1] = total_cost_2;
    }

    vector<Coordinates> notDominated;
    notDominated.push_back(*(new Coordinates(result[0][0], result[0][1])));

    for(int k = 1; k < 500 ; ++k){
        unsigned l = 0;
        bool determined = false;

        while(l < notDominated.size() && determined == false){
            if( (result[k][0] > notDominated.at(l).getCol()  && result[k][1] >notDominated.at(l).getRow() )) {

                determined = true;

            }else if( result[k][0] < notDominated.at(l).getCol()  && result[k][1] < notDominated.at(l).getRow() ){
                notDominated.at(l).setCol(result[k][0]);
                notDominated.at(l).setRow(result[k][1]);

                vector<int> toDelete;
                for(unsigned ind = l+1 ; ind < notDominated.size(); ++ind){
                    if( (result[k][0] <  notDominated.at(ind).getCol() && result[k][1] < notDominated.at(ind).getRow()) ){
                        toDelete.push_back(ind);
                    }
                }

                for(unsigned del=0; del < toDelete.size(); ++del){
                    notDominated.erase(notDominated.begin()+toDelete.at(del)-del);
                }
                determined = true;

            }else{
                ++l;
            }
        }

        if(determined == false){
            notDominated.push_back(*(new Coordinates(result[0][0], result[0][1])));
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
     
     //offline500_pareto
    string fileName =   "../data/results/offlinePareto500_test.txt";
     ofstream file2(fileName);

    if(!file2.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement" << endl;
    }else{
        for(unsigned j=0; j < notDominated.size(); ++j){
            file2 << notDominated.at(j).getCol() << " " << notDominated.at(j).getRow() << endl;
        }
    }
 
//       trySaveParetoToTxt(notDominated, "../data/results/offline500_Pareto_test.txt");
}
