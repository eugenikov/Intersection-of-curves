#ifndef Geom_Algo_H
#define Geom_Algo_H

class CPnt;
class CCurve;

class Geom_Algo
{
public:
    //It returns true and thePnts contains the collection of intersection points if method finds intersections.
    //It returns false if method doesn't find the intersection. Then thePnts may either contain two nearest points
    //([0] - for first curve, [1] - for second curve) or empty container in case of methods fails.
    static bool Intersection (const CCurve& theFirstCurve, const CCurve& theSecondCurve, std::vector <CPnt>& thePnts, double thePrecision);

    //Tridiagonal matrix algorithm is an algorithm for solving systems.
    //C - the lower diagonal, D - the main diagonal, E - the upper diagonal, B - the right part of system.
    //It returns an empty vector if sizes of the source vectors are incorrect.
    static std::vector<double> TMAlgorithm (const std::vector<double>& theC, const std::vector<double>& theD, 
        const std::vector<double>& theE, const std::vector<double>& theB);
};

#endif
