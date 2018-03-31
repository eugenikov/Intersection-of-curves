#ifndef CPnt_H
#define CPnt_H

#include <array>

class CPnt
{
public:
    enum {X, Y};

    std::array <double, 2> myCoordinates;

    CPnt();

    CPnt (double theX, double theY);

    //It computes a distance from thePnt to this point.
    double Distance (const CPnt& thePnt) const;

    //It computes a distance from (0, 0) to this point.
    double Magnitude() const;

    CPnt operator+(const CPnt & thePnt) const;
    CPnt operator-(const CPnt & thePnt) const;
};

#endif
