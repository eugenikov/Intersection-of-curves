#include <iostream>

#include "CSplineCurve.h"
#include "CPnt.h"
#include "Geom_Algo.h"
#include "Errors.h"
#include "Visitor.h"

CSplineCurve::CSplineCurve (const std::vector <std::array <double, 4>>& theXCoefficients,
    const std::vector <std::array <double, 4>>& theYCoefficients,
    const std::vector <double>& theDeltaT) : myXCoefficients (theXCoefficients),
    myYCoefficients (theYCoefficients),
    myTSegments (theDeltaT)
{
    if (theDeltaT.size() < 1 || theXCoefficients.size() < 1 || theYCoefficients.size() < 1) {
        std::cout << "An error during creation of spline." << std::endl;
        throw GenerateError();
    }

    myCoefOfParameterization = 0;

    for (size_t i = 0; i < theDeltaT.size(); ++i) {
        myCoefOfParameterization += theDeltaT[i];
    }
}

CSplineCurve::CSplineCurve (const std::vector <CPnt>& theControlPoints) : CSplineCurve (theControlPoints, {0, 0, 0, 0})
{}

CSplineCurve::CSplineCurve (const std::vector <CPnt>& theControlPoints, const ConditionType& theConditions)
{
    size_t aCNum = theControlPoints.size();

    if (aCNum < 2) {
        std::cout << "Error during creation of spline." << std::endl;
        throw GenerateError();
    }

    myTSegments.reserve (aCNum - 1);

    myCoefOfParameterization = 0;
    for (size_t i = 0; i < aCNum - 1; ++i) {
        myTSegments.push_back (theControlPoints[i].Distance (theControlPoints[i + 1]));
        myCoefOfParameterization += myTSegments[i];
    }

    if (aCNum == 2) {
        for (size_t i = 0; i < 2; ++i) {
            auto& aCoefficients = i == 0 ? myXCoefficients : myYCoefficients;
            aCoefficients.reserve (1);
            std::array <double, 4> aCoeff;
            aCoeff[A] = theControlPoints[1].myCoordinates[i];
            aCoeff[B] = (theControlPoints[1].myCoordinates[i] - theControlPoints[0].myCoordinates[i]) / myTSegments[0];
            aCoeff[C] = theConditions[0][i];
            aCoeff[D] = (theConditions[i][1] - theConditions[i][0]) / myTSegments[0];
            aCoefficients.push_back (aCoeff);
        }
        return;
    }

    if (aCNum == 3) {
        for (size_t i = 0; i < 2; ++i) {
            auto& aCoefficients = i == 0 ? myXCoefficients : myYCoefficients;
            aCoefficients.reserve (2);
            std::array <double, 4> aFirstCoeff;
            std::array <double, 4> aSecondCoeff;

            aFirstCoeff[A]  = theControlPoints[1].myCoordinates[i];
            aSecondCoeff[A] = theControlPoints[2].myCoordinates[i];

            aFirstCoeff[C] = (6 * ((theControlPoints[2].myCoordinates[i] - theControlPoints[1].myCoordinates[i]) / (myTSegments[1]) -
                                   (theControlPoints[1].myCoordinates[i] - theControlPoints[0].myCoordinates[i]) / (myTSegments[0])) -
                                    theConditions[1][i] * myTSegments[1] -
                                    theConditions[0][i] * myTSegments[0]) / ( 2 * (myTSegments[1] + myTSegments[0]));
            aSecondCoeff[C] = theConditions[1][i];

            aFirstCoeff[B] = (theControlPoints[1].myCoordinates[i] - theControlPoints[0].myCoordinates[i]) / myTSegments[0] + 
                              myTSegments[0] * (2 * aFirstCoeff[C] + theConditions[0][i]) / 6;
            aSecondCoeff[B] = (theControlPoints[2].myCoordinates[i] - theControlPoints[1].myCoordinates[i]) / myTSegments[1] + 
                               myTSegments[1] * (2 * aSecondCoeff[C] + aFirstCoeff[C]) / 6;

            aFirstCoeff[D] = (aFirstCoeff[C] - theConditions[0][i]) / myTSegments[0];
            aSecondCoeff[D] = (aSecondCoeff[C] - aFirstCoeff[C]) / myTSegments[1];

            aCoefficients.push_back (aFirstCoeff);
            aCoefficients.push_back (aSecondCoeff);
        }
        return;
    }

    //Filling system's coefficients for solving by TDAlgorithm.
    size_t n = aCNum - 2; //Size of Matrix.

    //The left parts of systems are identity for both X(t) and Y(t).
    std::vector <double> aC  (n - 1); //The lower diagonal.
    std::vector <double> aD  (n);     //The main  diagonal.
    std::vector <double> anE (n - 1); //The upper diagonal.

    //There are two vectors for right parts.
    std::vector <double> aB1 (n);
    std::vector <double> aB2 (n);

    enum {X, Y};

    //Filling the right parts of systems.
    auto FillRightPart = [&] (size_t i, int XorY, bool WithBoundary) {

        double aConditionValue (0);
        if (WithBoundary) {
             //Left or Right boundary value.
            aConditionValue = theConditions[i == 0 ? i : 1][XorY] * myTSegments[i];
        }

        std::vector <double>& aB = XorY == X ? aB1 : aB2;

        aB[i] = 6 * ((theControlPoints[i + 2].myCoordinates[XorY] - theControlPoints[i + 1].myCoordinates[XorY]) / (myTSegments[i + 1]) -
                     (theControlPoints[i + 1].myCoordinates[XorY] - theControlPoints[i].myCoordinates[XorY]) / (myTSegments[i])) - aConditionValue;

    };

    for (size_t i = 0; i < n; ++i) {
        bool anIsBoundary = false;
        if (i == 0 || i == n - 1) {
            anIsBoundary = true;
        }
        FillRightPart (i, X, anIsBoundary);
        FillRightPart (i, Y, anIsBoundary);
    }

    for (size_t i = 0; i < n - 1; ++i) {
        aD[i] = 2 * (myTSegments[i] + myTSegments[i + 1]);
        aC[i] = myTSegments[i];
        anE[i] = myTSegments[i + 1];
    }

    aD[n - 1] = 2 * (myTSegments[n - 1] + myTSegments[n]);

    const auto& aCX = Geom_Algo::TMAlgorithm (aC, aD, anE, aB1);
    const auto& aCY = Geom_Algo::TMAlgorithm (aC, aD, anE, aB2);


    myXCoefficients.reserve (n + 1);
    myYCoefficients.reserve (n + 1);

    //Filling the final spline's coefficients.
    auto FillCoef = [&] (size_t i, int XorY) {
        std::array <double, 4> aCoeff;

        const auto& aC = XorY == X ? aCX : aCY;

        aCoeff[A] = theControlPoints[i + 1].myCoordinates[XorY];

        aCoeff[B] = (theControlPoints[i + 1].myCoordinates[XorY] - theControlPoints[i].myCoordinates[XorY]) / myTSegments[i] +
                    (i == 0 ? myTSegments[i] * (2 * aC[i] + theConditions[0][XorY]) / 6 :     //The left segment
                     i == n ? myTSegments[i] * (2 * theConditions[1][XorY] + aC[i - 1]) / 6 : //The right segment
                              myTSegments[i] * (2 * aC[i] + aC[i - 1]) / 6);

        aCoeff[C] = i == n ?  theConditions[1][XorY] : aC[i];

        aCoeff[D] = i == 0 ? (aC[i] - theConditions[0][XorY]) / myTSegments[i] :     //The left segment
                    i == n ? (theConditions[1][XorY] - aC[i - 1]) / myTSegments[i] : //The right segment
                             (aC[i] - aC[i - 1]) / myTSegments[i];

        if (XorY == X) {
            myXCoefficients.push_back (aCoeff);
        } else {
            myYCoefficients.push_back (aCoeff);
        }
    };


    for (size_t i = 0; i < n + 1; ++i) {
        FillCoef (i, X);
        FillCoef (i, Y);
    }
}

size_t CSplineCurve::GetIndex (double t, double& theTofSegment) const
{
    theTofSegment = 0;

    //Case of out of range
    size_t anIndex = myTSegments.size() - 1;

    for (size_t i = 0, size = myTSegments.size(); i < size; ++i) {
        theTofSegment += myTSegments[i];
        if (theTofSegment > t) {
            anIndex = i;
            break;
        }
    }
    return anIndex;
}

CPnt CSplineCurve::Value (double t) const
{
    double aCurrentT;

    size_t anIndex = GetIndex (t, aCurrentT);

    const auto& aXCoef = myXCoefficients[anIndex];
    const auto& aYCoef = myYCoefficients[anIndex];
    
    double aDelta = (t - aCurrentT);

    double aX = aXCoef[A] + aXCoef[B] * aDelta + aXCoef[C] * aDelta * aDelta / 2 + aXCoef[D] * aDelta * aDelta * aDelta / 6;
    double aY = aYCoef[A] + aYCoef[B] * aDelta + aYCoef[C] * aDelta * aDelta / 2 + aYCoef[D] * aDelta * aDelta * aDelta / 6;

    return CPnt (aX, aY);
}

CPnt CSplineCurve::FirstDerivative (double t) const
{
    double aCurrentT;

    size_t anIndex = GetIndex (t, aCurrentT);

    const auto& aXCoef = myXCoefficients[anIndex];
    const auto& aYCoef = myYCoefficients[anIndex];
    
    double aDelta = (t - aCurrentT);

    double aX = aXCoef[B] + aXCoef[C] * aDelta + aXCoef[D] * aDelta * aDelta / 3;
    double aY = aYCoef[B] + aYCoef[C] * aDelta + aYCoef[D] * aDelta * aDelta / 3;

    return CPnt (aX, aY);
}

CPnt CSplineCurve::SecondDerivative (double t) const
{
    double aCurrentT;

    size_t anIndex = GetIndex (t, aCurrentT);

    const auto& aXCoef = myXCoefficients[anIndex];
    const auto& aYCoef = myYCoefficients[anIndex];
    
    double aDelta = (t - aCurrentT);

    double aX = aXCoef[C] + aXCoef[D] * 2 * aDelta / 3;
    double aY = aYCoef[C] + aYCoef[D] * 2 * aDelta / 3;

    return CPnt (aX, aY);
}

void CSplineCurve::Accept (Visitor& theVisitor) const {
    theVisitor.Visit (*this);
}
