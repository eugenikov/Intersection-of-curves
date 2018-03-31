#include <cmath>

#include "CPnt.h"

CPnt::CPnt()
{
    for (size_t i = 0; i < 2; ++i) {
        myCoordinates[i] = 0;
    }
}

CPnt::CPnt (double theX, double theY)
{
    myCoordinates[X] = theX;
    myCoordinates[Y] = theY;
}
double CPnt::Magnitude() const
{
    CPnt aPnt (0, 0);
    return Distance (aPnt);
}

double CPnt::Distance (const CPnt& thePnt) const
{
    const auto& aXY = thePnt.myCoordinates;
    return std::sqrt ((aXY[X] - myCoordinates[X]) * (aXY[X] - myCoordinates[X]) +
                      (aXY[Y] - myCoordinates[Y]) * (aXY[Y] - myCoordinates[Y]));
}

CPnt CPnt::operator+(const CPnt & thePnt) const
{
    return CPnt (myCoordinates[X] + thePnt.myCoordinates[X],
                 myCoordinates[Y] + thePnt.myCoordinates[Y]);
}

CPnt CPnt::operator-(const CPnt & thePnt) const
{
    return CPnt (myCoordinates[X] - thePnt.myCoordinates[X],
                 myCoordinates[Y] - thePnt.myCoordinates[Y]);
}
