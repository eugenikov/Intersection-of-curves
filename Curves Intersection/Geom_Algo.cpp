#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include "Geom_Algo.h"
#include "CCurve.h"
#include "CPnt.h"


namespace {
bool MethodNewtonaRaphsonaSolveSystem (const CCurve& theFirstCurve, const CCurve& theSecondCurve, double& t1, double& t2, double thePrecision)
{
    double PrevT1 = t1;
    double PrevT2 = t2;

    CPnt aPrevPnt1;
    CPnt aPrevPnt2;

    double F1;
    double F2;

    CPnt aCurPnt1;
    CPnt aCurPnt2;

    int i = 0;
    do {
        if (++i == 1000) {
            return false;
        }
        aPrevPnt1 = theFirstCurve.Value (t1);
        aPrevPnt2 = theSecondCurve.Value (t2);

        PrevT1 = t1;
        PrevT2 = t2;

        auto aDP1 = theFirstCurve.FirstDerivative (t1);
        auto aDP2 = theSecondCurve.FirstDerivative (t2);

        double F11 =  aDP1.myCoordinates[0];
        double F12 = -aDP2.myCoordinates[0];

        double F21 =  aDP1.myCoordinates[1];
        double F22 = -aDP2.myCoordinates[1];

        F1 = aPrevPnt1.myCoordinates[0] - aPrevPnt2.myCoordinates[0];
        F2 = aPrevPnt1.myCoordinates[1] - aPrevPnt2.myCoordinates[1];

        double D1 = (-F2 + (F22 * F1) / F12) / (F21 - F11 * F22 / F12);
        double D2 = (-F1 - F11 * D1) / F12;

        t2 = PrevT2 + D2;
        t1 = PrevT1 + D1;

        aCurPnt1 = theFirstCurve.Value (t1);
        aCurPnt2 = theSecondCurve.Value (t2);

    } while (std::abs (t1 - PrevT1) > thePrecision || std::abs (t2 - PrevT2) > thePrecision ||
             std::abs (aCurPnt1.myCoordinates[0] - aPrevPnt1.myCoordinates[0]) > thePrecision ||
             std::abs (aCurPnt1.myCoordinates[1] - aPrevPnt1.myCoordinates[1]) > thePrecision ||
             std::abs (aCurPnt2.myCoordinates[0] - aPrevPnt2.myCoordinates[0]) > thePrecision ||
             std::abs (aCurPnt2.myCoordinates[1] - aPrevPnt2.myCoordinates[1]) > thePrecision);

    if (std::abs (F1) > thePrecision && std::abs (F2) > thePrecision) {
        return false;
    }
    return true;

}
bool MethodNewtonaRaphsonaOptimization (const CCurve& theFirstCurve, const CCurve& theSecondCurve, double& t1, double& t2, double thePrecision)
{

    double PrevT1 = t1;
    double PrevT2 = t2;

    CPnt aPrevPnt1;
    CPnt aPrevPnt2;

    double F1;
    double F2;

    size_t i = 0;
    do {
        if (++i == 1000) {
            return false;
        }
        aPrevPnt1 = theFirstCurve.Value (t1);
        aPrevPnt2 = theSecondCurve.Value (t2);

        PrevT1 = t1;
        PrevT2 = t2;

        auto aPnt1 = aPrevPnt1.myCoordinates;
        auto aPnt2 = aPrevPnt2.myCoordinates;

        auto aDP1 = theFirstCurve.FirstDerivative (t1);
        auto aDP2 = theSecondCurve.FirstDerivative (t2);

        auto aCordD1 = aDP1.myCoordinates;
        auto aCordD2 = aDP2.myCoordinates;

        auto aSD1 = theFirstCurve.SecondDerivative (t1).myCoordinates;
        auto aSD2 = theSecondCurve.SecondDerivative (t2).myCoordinates;

        double F11 = 2 * aCordD1[0] * aCordD1[0] + 2 * (aPnt1[0] - aPnt2[0]) * aSD1[0] + 2 *aCordD1[1] * aCordD1[1] + 2 * (aPnt1[1] - aPnt2[1])* aSD1[1];

        double F12 = -2 * aCordD2[0] * aCordD1[0] - 2 * aCordD2[1] * aCordD1[1];

        double F21 = F12;

        double F22 = 2 * aCordD2[0] * aCordD2[0] - 2 * (aPnt1[0] - aPnt2[0]) * aSD2[0] + 2 * aCordD2[1] * aCordD2[1] - 2 * (aPnt1[1] - aPnt2[1])* aSD2[1];

        F1 = 2 * (aPnt1[0] - aPnt2[0]) * aCordD1[0] + 2 * (aPnt1[1] - aPnt2[1]) * aCordD1[1];
        F2 = -2 * (aPnt1[0] - aPnt2[0]) * aCordD2[0] - 2 * (aPnt1[1] - aPnt2[1]) * aCordD2[1];

        double D1 = (-F2 + (F22 * F1) / F12) / (F21 - F11 * F22 / F12);
        double D2 = (-F1 - F11 * D1) / F12;

        t2 = PrevT2 + D2;
        t1 = PrevT1 + D1;

    } while (std::abs (t1 - PrevT1) > thePrecision * 0.01 || std::abs (t2 - PrevT2) > thePrecision * 0.01);

    CPnt aSolPnt1 = theFirstCurve.Value (t1);
    CPnt aSolPnt2 = theSecondCurve.Value (t2);

    if (aSolPnt1.Distance (aSolPnt2) > thePrecision) {
        return false;
    }
    return true;
}
}
std::vector<double> Geom_Algo::TMAlgorithm (const std::vector <double>& theC, const std::vector <double>& theD,
    const std::vector <double>& theE, const std::vector <double>& theB)
{
    if (theD.size () <= 1 || theD.size() - 1 != theE.size() || theD.size() - 1 != theC.size() || theD.size() != theB.size()) {
        std::cout << "Incorrect size of vectors for solving the system." << std::endl;
        std::vector<double> anEmptyVec;
        return anEmptyVec;
    }

    //support values
    std::vector <double> anA (theD.size() - 1);
    std::vector <double> aB  (theD.size() - 1);

    //Direct process
    anA[0] = -theE[0] / theD[0];
    aB[0] = theB[0] / theD[0];

    for (size_t i = 1; i < theD.size() - 1; ++i) {
        anA[i] = -theE[i] / (theD[i] + theC[i] * anA[i - 1]);
        aB[i] = (-theC[i] * aB[i - 1] + theB[i]) / (theD[i] + theC[i] * anA[i - 1]);
    }

    std::vector<double> aSolution (theB.size());

    //Inverse process
    aSolution[theB.size() - 1] = (-theC[theC.size() - 1] * aB[aB.size() - 1] + theB[theB.size() - 1]) /
                                 (theD[theD.size() - 1] + theC[theC.size() -1] * anA[anA.size() - 1]);

    for (int i = static_cast <int> (theD.size()) - 2; i >= 0; --i) {
        aSolution[i] = anA[i] * aSolution[i + 1] + aB[i];
    }

    return aSolution;
}

bool Geom_Algo::Intersection (const CCurve& theFirstCurve, const CCurve& theSecondCurve, std::vector <CPnt>& thePnts, double thePrecision)
{
    enum {X, Y};

    typedef CPnt Vec;
    //Collecting the starting points for the iteration method of solving system.

    //Golovanov N.N. Geometricheskoe modelirovanie 2002.
    //Chapter 4.

    //Formula 4.2.3, page 212
    //Alpha can be from pi/20 to pi/10 and characterizes a
    //deflection's angle of tanget vector after delta step.
    auto ComputeDelta = [] (double t, const CCurve& theCurve) {
        const double anAlpha = M_PI / 20;

        auto aFirstDerivative = theCurve.FirstDerivative (t);
        auto aSecondDerivative = theCurve.SecondDerivative (t);

        auto aC1 = aFirstDerivative.myCoordinates;
        auto aC2 = aSecondDerivative.myCoordinates;
        double asf = std::abs (aC1[X] * aC2[Y] - aC1[Y] * aC2[X]);
        return anAlpha * aFirstDerivative.Magnitude() * aFirstDerivative.Magnitude() / std::abs (aC1[X] * aC2[Y] - aC1[Y] * aC2[X]);
    };

    auto ScalarMultiply = [] (const Vec& theVec1, const Vec& theVec2) {
        return theVec1.myCoordinates[X] * theVec2.myCoordinates[X] +
               theVec1.myCoordinates[Y] * theVec2.myCoordinates[Y];
    };

    //To find the start parameters, t1 and t2 are iterated with delta (Formula 4.2.3) and computed the
    //intersection's parameters of tanget lines (Formula 4.8.1, 4.8.2). Current parameters will be used
    //as start parameters for iteration's method.
    std::vector <std::array <double, 2>> aStartParameters;
    for (double t1 = 0, dt1 = 0; t1 < theFirstCurve.GetRange(); t1+=dt1) {
        dt1 = ComputeDelta (t1, theFirstCurve);
        for (double t2 = 0, dt2 = 0; t2 < theSecondCurve.GetRange(); t2+=dt2) {
            dt2 = ComputeDelta (t2, theSecondCurve);

            auto aP1 = theFirstCurve.Value (t1);
            auto aP2 = theSecondCurve.Value (t2);

            auto aCD1 = theFirstCurve.FirstDerivative (t1);
            auto aCD2 = theSecondCurve.FirstDerivative (t2);

            auto ComputeNormal = [] (const Vec& theVec) {
                double k = 1 / (std::sqrt (theVec.myCoordinates[X] * theVec.myCoordinates[X] + theVec.myCoordinates[Y] * theVec.myCoordinates[Y]));
                double x = -k * theVec.myCoordinates[Y];
                double y = k * theVec.myCoordinates[X];
                return Vec (x, y);
            };

            Vec aNormal1 = ComputeNormal (aCD1);
            Vec aNormal2 = ComputeNormal (aCD2);

            //Parameters of intersection's lines (Formula 4.8.3, 4.8.4, page 241)
            double aW1 = ScalarMultiply (aNormal2, aP2 - aP1) / ScalarMultiply (aNormal2, aCD1);
            double aW2 = ScalarMultiply (aNormal1, aP1 - aP2) / ScalarMultiply (aNormal1, aCD2);

            if (-dt1 <= aW1 && aW1 <= dt1 && -dt2 <= aW2 && aW2 <= dt2) {
                aStartParameters.push_back ({t1 + aW1, t2 + aW2});
            }
        }
    }

    //Adding boundary start parameters.
    aStartParameters.push_back ({0, 0});
    aStartParameters.push_back ({theFirstCurve.GetRange(), theSecondCurve.GetRange()});

    std::vector <std::array <double, 2>> anIntersectionPoints;
    std::vector <std::array <double, 3>> aDistancePoints;
    bool aHaveInterSection = false;

    //The optimization's problem is solved at the begining to find a distnace of curves.
    //If solution of optimazation's problem is an intersection of curves then find all intersections by solving the system c1(t) - c2(t) = 0.
    //The Newton-Rapshon's method is used in both cases.
    for (size_t i = 0; i < aStartParameters.size(); ++i) {
        double t1 = aStartParameters[i][0];
        double t2 = aStartParameters[i][1];
        if (aHaveInterSection || !MethodNewtonaRaphsonaOptimization (theFirstCurve, theSecondCurve, t1, t2, thePrecision)) {
            double rt1 = aStartParameters[i][0];
            double rt2 = aStartParameters[i][1];
            if (MethodNewtonaRaphsonaSolveSystem (theFirstCurve, theSecondCurve, rt1, rt2, thePrecision)) {
                t1 = rt1;
                t2 = rt2;
            }
        }

        if (theFirstCurve.Value (t1).Distance (theSecondCurve.Value (t2)) < thePrecision) {
            anIntersectionPoints.push_back ({t1, t2});
            aHaveInterSection = true;
        } else if (!aHaveInterSection) {
            aDistancePoints.push_back ({theFirstCurve.Value (t1).Distance (theSecondCurve.Value (t2)), t1, t2});
        }
    }

    //ksi must be > eps
    double ksi = thePrecision * 10;

    //Sort found solutions to erase equals solutions.
    std::sort (anIntersectionPoints.begin(), anIntersectionPoints.end(), [] (const std::array <double, 2>& a, const std::array <double, 2>& b) {
            return a[0] < b[0];});
    std::vector <std::array <double, 2>> aPoints;

    for (int i = 0; i < static_cast <int> (anIntersectionPoints.size()) - 1; ++i) {
        if (std::abs (anIntersectionPoints[i][0] - anIntersectionPoints[i + 1][0]) < ksi &&
            std::abs (anIntersectionPoints[i][1] - anIntersectionPoints[i + 1][1]) < ksi) {
            continue;
        }
        aPoints.push_back (anIntersectionPoints[i]);
    }

    if (anIntersectionPoints.size()) {
        aPoints.push_back (anIntersectionPoints[anIntersectionPoints.size() - 1]);
    }

    std::cout.precision (16);
    if (aPoints.size()) {
        std::cout << "Intersections was found." << std::endl;
        for (size_t i = 0; i < aPoints.size (); ++i) {
            std::cout << "Point " << i << ":" << std::endl;
            std::cout << theFirstCurve.Value (aPoints[i][0]).myCoordinates[0] << " " << theFirstCurve.Value (aPoints[i][0]).myCoordinates[1] << std::endl; 
        }
        return true;
    } else if (aDistancePoints.size()) {
        double aMinimumDistance = aDistancePoints[0][0];
        size_t anIndexOfPnt = 0;
        for (size_t i = 1; i < aDistancePoints.size(); ++i) {
            if (aMinimumDistance > aDistancePoints[i][0]) {
                aMinimumDistance = aDistancePoints[i][0];
                anIndexOfPnt = i;
            }
        }

        std::cout << "Minimum distance = ";
        std::cout << aMinimumDistance << std::endl;
        std::cout << "Point of the first curve: " << theFirstCurve.Value (aDistancePoints[anIndexOfPnt][1]).myCoordinates[0] << " " <<
                                                     theFirstCurve.Value (aDistancePoints[anIndexOfPnt][1]).myCoordinates[1] << std::endl;

        std::cout << "Point of the second curve: " << theSecondCurve.Value (aDistancePoints[anIndexOfPnt][2]).myCoordinates[0] << " " <<
                                                      theSecondCurve.Value (aDistancePoints[anIndexOfPnt][2]).myCoordinates[1] << std::endl;

        return false;
    }

    std::cout << "Intersection wasn't found. Distance also wasn't found. thePnts is empty." << std::endl;
    return false;
}
