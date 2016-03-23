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
{

}

Instance::Instance(const Instance& other):
    nb_cities(other.nb_cities), cities_positions(other.cities_positions)
{

}

Instance& Instance::operator=(const Instance& other)
{
    nb_cities=other.nb_cities;
    cities_positions= other.cities_positions;
    
    return *this;
}



bool Instance::tryLoadFile(const string& fileName){

    ifstream f(fileName.c_str());

    if(!f){
        cerr << "Erreur pendant l'ouverture du fichier" << endl;
        return false;
    }else{
        string line;
	
	clog << "Lecture fichier :"<< endl;
	for (int i= 1 ; i <= 3; ++i){
	    getline(f,line);
	    clog << "Ligne "<< i<< " contient : "<< line<< endl;
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
	
	for (int i= 5 ; i <= 6; ++i){
	    getline(f,line);
	    clog << "Ligne "<< i<< " contient : "<< line<< endl;
	}
	
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

    return true;

}

const Coordinates& Instance::get_city(unsigned int id) const
{
    if (id <= nb_cities && id >0){
	return cities_positions[id];
    }
    else return *(new Coordinates(-1, -1));
}

unsigned int Instance::get_nbcities() const
{
  return nb_cities;
}

ostream& Instance::print(ostream& out) const
{
    for(unsigned int i=1; i <= nb_cities; ++i) {
       out<< "id[" << i << "] (" << cities_positions[i].col << ", " << cities_positions[i].row << ")\n" << endl;
    }
    
    return out;
}
 
float Instance::calc_distance(const Coordinates& A, const Coordinates& B)
{
  return sqrt(pow(A.col - B.col, 2) + pow(A.row - B.row, 2));
}

float Instance::calc_distance(int A, int B)
{
    return calc_distance(cities_positions[A], cities_positions[B]);
}


vector< vector<float> >& Instance::matriceDistance(Instance inst) {
    vector< vector<float> >& mat_dist= *(new vector< vector<float> >);
    
    for (int i= 1; i<= 100; ++i){
	mat_dist.push_back(vector<float>());
	for (int j= 1; j<= 100; ++j){
	    mat_dist[i].push_back(calc_distance(i, j));
	}
    }
    
    return mat_dist;
}

