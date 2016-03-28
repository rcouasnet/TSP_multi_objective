#ifndef EVALUATION_H
#define EVALUATION_H

class Evaluation
{
private:
    double val_obj1;
    double val_obj2;
    
public:
    Evaluation(double _val_obj1, double _val_obj2) :
	val_obj1(_val_obj1), val_obj2(_val_obj2) {};
	
    double get_val1() const {  return val_obj1 ;}
    double get_val2() const { return val_obj2 ;}
    
    /**
     * Regarde si l'évaluation est dominée par une autre
     * @param other : l'autre évaluation
     * @return true si la solution est dominée (en minimisation)
     */
    bool is_dominated(const Evaluation& other) const;
    
    void set_obj1(double cost) { val_obj1= cost; }
    void set_obj2(double cost) { val_obj2= cost; }
};

#endif // EVALUATION_H
