#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates
{

public:
    int col;
    int row;

public:
    Coordinates(int _x, int _y);
    Coordinates(const Coordinates& other);
    virtual bool operator==(const Coordinates& other) const;
    double getCol(){ return col; }
    double getRow(){ return row; }
//     void setCol(double _x){ col = _x; }
//     void setRow(double _y){ row = _y; }
	
};

#endif // COORDINATES_H
