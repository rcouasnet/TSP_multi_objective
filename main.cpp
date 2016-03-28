#include <iostream>
#include "objective.h"
#include "coordinates.h"
#include "instance_tsp.h"


using namespace std;
void launch_menu(int argc, char** argv)
{
//     /* MAIN FONCTIONNEL MAIS PAS PRATIQUE POUR TEST

    if(argc != 3) {
	std::cerr << " arg[0] = nom du programme, arg[1] = fichier A, arg[2] = fichier B." << std::endl;
    } else {
	 
	std::string File_A(argv[1]);
	std::string File_B(argv[2]);
	
	InstanceTsp *tsp = new InstanceTsp(File_A, File_B);
	tsp->initSeeds();
	tsp->setBothCostMatrices();
	
	int choix = 0;
	cout << "Que voulez-vous faire ?" << endl;
	cout << "1 - Filtre Offline" << endl;
	cout << "2 - Filtre Online" << endl;
	cout << "3 - mTSP" << endl;
	cin >> choix;
	while(choix<1 || choix >3) {
	cout << "Erreur, veuillez ressaisir votre choix." << endl;
	cin >> choix; 
	}
	switch(choix){
	    case 1 : tsp->offlineFilter();
	    break;
	    case 2 : tsp->onlineFilter();
	    break;
	    case 3 : unsigned iteration;
	    cout << "Veuillez entrez le nombre d'itération à réaliser : " << endl;
	    cin >> iteration;
	    tsp->mTSP(iteration);
	    break;
	}
    }
    
//   */
}

int main(int argc, char **argv) {

    launch_menu(argc, argv);
    
//   Objective i1, i2;
//   std::string File_A = "../data/kroA100.tsp";
//   std::string File_B = "../data/kroB100.tsp";
// 
//   if (i1.tryLoadFile(File_A)){
// #if DEBUG_CREATION
//     clog << "lecture réussie : " << i1 << endl;
// #endif
//   }
//   
//   if (i2.tryLoadFile(File_B)){
// #if DEBUG_CREATION
//     clog << "lecture réussie : " << i2 << endl;
// #endif
//   }
//   
//   InstanceTsp *tsp = new InstanceTsp(File_A, File_B);
//   tsp->initSeeds();
//   tsp->setBothCostMatrices();
  
  //tsp->offlineFilter();
  //tsp->onlineFilter();
//   tsp->mTSP(100);
  
  return 0;
}

