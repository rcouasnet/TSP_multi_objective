#include "evaluation.h"

bool Evaluation::is_dominated(const Evaluation& other) const
{
    return ( (other.get_val1() < val_obj1) && (other.get_val2() < val_obj2) );
}
