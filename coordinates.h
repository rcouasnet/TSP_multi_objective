#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates
{

public:
    double col;
    double row;

public:
    Coordinates(double _x, double _y);
    Coordinates(const Coordinates& other);
    virtual bool operator==(const Coordinates& other) const;
    double getCol(){ return col; }
    double getRow(){ return row; }
    void setCol(double _x){ col = _x; }
    void setRow(double _y){ row = _y; }
	
};

#endif // COORDINATES_H
