#include "instance_tsp.h"

#include <math.h>

using namespace std;

vector<string>& explode(const string &str){

    istringstream split(str);
    vector< string >* tokens = new vector<string>;

    for(string each; getline(split, each, ' '); tokens->push_back( each.c_str()) );

    return *tokens;
}


Objective::Objective()
{}

Objective::Objective(const Objective& other):
    nb_cities(other.nb_cities), cities_positions(other.cities_positions),
    cost_matrice(other.cost_matrice)
{}

Objective& Objective::operator=(const Objective& other)
{
    nb_cities=other.nb_cities;
    cities_positions= other.cities_positions;
    cost_matrice= other.cost_matrice;
    
    return *this;
}

const Coordinates& Objective::get_city(unsigned int id) const
{
    if (id <= nb_cities && id >0){
	return cities_positions[id];
    }
    else return *(new Coordinates(-1, -1));
}

void Objective::initCostMatrice()
{
    cost_matrice.resize(nb_cities +1);
    for (int i= 1; i<= 100; ++i){
	cost_matrice[i].resize(nb_cities +1);
    }
}

ostream& Objective::print(ostream& out) const
{
    for(unsigned int i=1; i <= nb_cities; ++i) {
       out<< "id[" << i << "] (" << cities_positions[i].col << ", " << cities_positions[i].row << ")" << endl;
    }
    
    return out;
}
 
double Objective::calcDistance(const Coordinates& A, const Coordinates& B)
{
    return sqrt(pow(A.col - B.col, 2) + pow(A.row - B.row, 2));
}

double Objective::calcDistance(double ind_A, double ind_B)
{
    return calcDistance(cities_positions[ind_A], cities_positions[ind_B]);
}

void Objective::setMatriceDistance()
{
    for (int i= 1; i<= 100; ++i){
	for (int j= i; j<= 100; ++j){
	    // Calcule de la distance et stockage dans les 2 cases
	    //	de la matrice correspondantes
	    double dist_i_j= calcDistance(i, j);
	    cost_matrice[i][j]=  dist_i_j;
	    cost_matrice[j][i]=  dist_i_j;
	}
	
	#if DEBUG_MAT_DIST
	    cout << "Taille ligne "<< i<<" de la matrice de distances : "<<
	    cost_matrice[i].size()<< endl;
	#endif
    }
    #if DEBUG_MAT_DIST
	cout << "Hauteur de la matrice de distances : "<< cost_matrice.size()<< endl;
    #endif
}

float Objective::get_distance(double ind_A, double ind_B) const
{
    #if DEBUG_MAT_DIST
	cout << "récupération de la distance entre "<< ind_A<< " et "<< ind_B<< endl;
    #endif
    return cost_matrice[ind_A][ind_B];
}

// ###################################
//	Lectures/Ecriture fichiers
// ###################################

bool Objective::tryLoadFile(const string& fileName){

    ifstream f(fileName.c_str());

    if(!f){
        cerr << "Erreur pendant l'ouverture du fichier" << endl;
        return false;
    }else{
// Lecture du nombre de villes
        string line;
	
	for (int i= 1 ; i <= 3; ++i){
	    getline(f,line);
    #if DEBUG_LOAD_FILE
	    clog << "Ligne "<< i<< " contient : "<< line<< endl;
    #endif
	}
	
	// Lecture de la ligne contenant le nombre de villes
	getline(f,line);
	#if DEBUG_LOAD_FILE
	    cout << line << endl;
	#endif	
	vector<string>& tokens_line = explode(line);
	/*
	 * tokens[0] : "DIMENSION:"
	 * tokens[1] : nombre de villes
	 */
	nb_cities= atoi(tokens_line[1].c_str());
	cities_positions.reserve(nb_cities +1);
	
	delete(&tokens_line);
	
// Lecture des lignes en commentaire
	for (int i= 5 ; i <= 6; ++i){
	    getline(f,line);
	    #if DEBUG_LOAD_FILE
		clog << "Ligne "<< i<< " contient : "<< line<< endl;
	    #endif
	}
	
// Lecture des positions des villes
        while(getline(f,line)){

            vector<string>& tokens = explode(line);
	    if (tokens[0] != "EOF"){
                /*
		 * tokens[0] : id
		 * tokens[1] : x
		 * tokens[2] : y
                 */
		int id= atoi(tokens[0].c_str());
		int x= atoi(tokens[1].c_str());
		int y= atoi(tokens[2].c_str());
		cities_positions[id].col =x;
		cities_positions[id].row= y;
            }
            
            delete(&tokens);
        }
    }
    
    initCostMatrice();

    return true;
}
