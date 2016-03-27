#include "coordinates.h"

Coordinates::Coordinates(double _x, double _y)
	:col(_x), row(_y)
{}

Coordinates::Coordinates(const Coordinates& other)
	:col(other.col), row(other.row)
{}

bool Coordinates::operator==(const Coordinates& other) const
{
	return (other.col==col && other.row==row);
}

/* parcours des 2 matrices dans l'ordre
 * 	calc de distance entre valeur 1 dela matA avec la valeur2
 * 	idem pour la B
 * 	on place les distances dans deux tableaux de valeurs
 * 	+ ne pas oublier distance entre prem et derniere pair
 * ces 2 tableaux de distances donne des coord tab[A],tab[B]
 */