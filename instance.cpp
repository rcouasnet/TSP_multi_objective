#include "instance.h"

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
    
    return this;
}



bool Instance::tryLoadFile(const string& fileName){

    ifstream f(fileName.c_str());

    if(!f){
        cerr << "Erreur pendant l'ouverture du fichier" << endl;
        return false;
    }else{
        string line;
        int indice = -1;
	
	clog << "Lecture fichier :"<< endl;
	for (int i= 1 ; i <= 6; ++i){
	    getline(f,line);
	    clog << "Ligne "<< i<< " contient : "<< line<< endl;
	}
	
	
	// Lecture de la première ligne renseignant nombre de lignes et nombre de colonnes
	getline(f,line);
	cout << line << endl;
	
	
	vector<string>& tokens_line = explode(line);
	/*
	 * tokens[0] : "DIMENSION:"
	 * tokens[1] : nombre de villes
	 */
	nb_cities= atoi(tokens_line[1].c_str());
	
	delete(&tokens_line);
	
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
		cities_postions[id].col =x;
		cities_postions[id].row= y;
            }
            
            delete(&tokens);
        }
    }

    return true;

}