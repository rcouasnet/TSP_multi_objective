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
    
bool InstanceTsp::trySaveParetoToTxt(vector<Coordinates> notDominated, const string& fileName)
{
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement" << endl;
        return false;
    }else{
        for(unsigned i=0; i < notDominated.size(); ++i){
            file << notDominated[i].col << " " << notDominated[i].row << endl;
        }
    }
    return true;
}



void InstanceTsp::offlineFilter()
{
    double result[500][2] = { { 0 } };

    vector<int> p;

    for(unsigned i= 0; i < 500; ++i) {
        generatePath(i);
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
            if( (result[k][0] > notDominated.at(l).col  && result[k][1] >notDominated.at(l).row )) {

                determined = true;

            }else if( result[k][0] < notDominated.at(l).col  && result[k][1] < notDominated.at(l).row ){
                notDominated.at(l).col= result[k][0];
                notDominated.at(l).row= result[k][1];

                vector<int> toDelete;
                for(unsigned ind = l+1 ; ind < notDominated.size(); ++ind){
                    if( (result[k][0] <  notDominated.at(ind).col && result[k][1] < notDominated.at(ind).row) ){
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
 
     //offline500 Pareto
     trySaveParetoToTxt(notDominated, "../data/results/offline500_Pareto_test.txt");
}


void InstanceTsp::onlineFilter()
{
//     pair<double,double> currentSol;
// 
//     vector<pair<double,double> >nonDominated;
// 
//     string instName =   "results/online/online500_"+instanceName+".txt";
//     ofstream file(instName);
// 
//     if(!file){
//         cerr << "Erreur de création du fichier" << endl;
//     }
//     else{
//         for(unsigned i=0; i < 500 ; ++i){
// 	  
//             generatePath(i);
// 
//             // Evaluating the path
//             initEvaluation();
// 
//             // Writing current solution in file online500 ...
//             file << total_cost_1<< " " << total_cost_2<< endl;
// 
//             // Testing the path with all the non-dominated ones
//             unsigned m = 0;
//             bool determined = false;
// 
//             while( m < nonDominated.size() && determined == false){
//                 if( (total_cost_1 > nonDominated.at(m).first && total_cost_2 > nonDominated.at(m).second ) ){
//                     determined = true;
//                 }
//                 else if( (total_cost_1 < nonDominated.at(m).first && total_cost_2 < nonDominated.at(m).second) ){
//                     nonDominated.at(m).first = total_cost_1;
//                     nonDominated.at(m).second = total_cost_2;
// 
//                     // Propagation
//                     vector<int> toDelete;
//                     for(unsigned ind = m+1 ; ind < nonDominated.size(); ++ind){
//                         if( (total_cost_1 <  nonDominated.at(ind).first && total_cost_2 < nonDominated.at(ind).second) ){
//                             toDelete.push_back(ind);
//                         }
//                     }
//                     for(unsigned del=0; del < toDelete.size(); ++del){
//                         nonDominated.erase(nonDominated.begin()+toDelete.at(del)-del);
//                     }
// 
//                     // End of propagation
//                     determined = true;
//                 }
//                 else{
//                     ++m;
//                 }
//             }
// 
//             if(determined == false){
//                 nonDominated.push_back(pair<double,double>(total_cost_1, total_cost_2));
//             }
//         }
// 
//         // Writing Pareto local front in onlinePareto500_instanceName.txt
//         string paretoFileName = "results/online/onlinePareto500_"+instanceName+".txt";
// 
//         ofstream fileP(paretoFileName);
// 
//         if(!fileP){
//             cerr << "Erreur de création de fichier" << endl;
//         }else{
//             for(unsigned j=0; j < nonDominated.size(); ++j){
//                 fileP << nonDominated[j].first << " " << nonDominated[j].second << endl;
//             }
//         }
//     }
}

