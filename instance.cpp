#include "instance.h"
#include <math.h>

using namespace std;

vector<string>& explode(const string &str){

    istringstream split(str);
    vector< string >* tokens = new vector<string>;

    for(string each; getline(split, each, ' '); tokens->push_back( each.c_str()) );

    return *tokens;
}


Instance::Instance()
{}

Instance::Instance(const Instance& other):
    nb_cities(other.nb_cities), cities_positions(other.cities_positions),
    cost_matrice(other.cost_matrice)
{}

Instance& Instance::operator=(const Instance& other)
{
    nb_cities=other.nb_cities;
    cities_positions= other.cities_positions;
    cost_matrice= other.cost_matrice;
    
    return *this;
}

const Coordinates& Instance::get_city(unsigned int id) const
{
    if (id <= nb_cities && id >0){
	return cities_positions[id];
    }
    else return *(new Coordinates(-1, -1));
}

void Instance::initCostMatrice()
{
    cost_matrice.resize(nb_cities +1);
    for (int i= 1; i<= 100; ++i){
	cost_matrice[i].resize(nb_cities +1);
    }
}

ostream& Instance::print(ostream& out) const
{
    for(unsigned int i=1; i <= nb_cities; ++i) {
       out<< "id[" << i << "] (" << cities_positions[i].col << ", " << cities_positions[i].row << ")" << endl;
    }
    
    return out;
}
 
float Instance::calcDistance(const Coordinates& A, const Coordinates& B)
{
    return sqrt(pow(A.col - B.col, 2) + pow(A.row - B.row, 2));
}

float Instance::calcDistance(int ind_A, int ind_B)
{
    return calcDistance(cities_positions[ind_A], cities_positions[ind_B]);
}

void Instance::setMatriceDistance()
{
    for (int i= 1; i<= 100; ++i){
	for (int j= i; j<= 100; ++j){
	    // Calcule de la distance et stockage dans les 2 cases
	    //	de la matrice correspondantes
	    float dist_i_j= calcDistance(i, j);
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

float Instance::get_distance(int ind_A, int ind_B) const
{
    #if DEBUG_MAT_DIST
	cout << "récupération de la distance entre "<< ind_A<< " et "<< ind_B<< endl;
    #endif
    return cost_matrice[ind_A][ind_B];
}

// ###################################
//	Lectures/Ecriture fichiers
// ###################################

bool Instance::tryLoadFile(const string& fileName){

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
	cout << line << endl;
	
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

bool Instance::trySaveToTxt(const string& fileName)
{
    ofstream file(fileName);

    if(!file.is_open()){
        cerr << "Erreur pendant l'ouverture du fichier d'enregistrement" << endl;
        return false;
    }else{
	
	for (unsigned i =1; i<= nb_cities; ++i){
	    ostringstream oss;
	    oss << cities_positions[i].col<< " "<< cities_positions[i].row<< endl;
	    
	    string line= oss.str();
	    
	    // Ecriture des coordonnées de la ville 
	    file << line;
	}
	
	file.close();
    }

    return true;
}
