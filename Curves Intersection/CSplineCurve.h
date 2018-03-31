#ifndef CSplineCurve_H
#define CSplineCurve_H

#include "CCurve.h"
#include <vector>

class CPnt;

class CSplineCurve : public CCurve
{
private:
    enum {A, B, C, D};

    std::vector <std::array <double, 4>> myXCoefficients;
    std::vector <std::array <double, 4>> myYCoefficients;

    std::vector <double> myTSegments;

public:
    const auto& GetXCoef () const { return myXCoefficients; }
    const auto& GetYCoef () const { return myYCoefficients; }
    const auto& GetDT () const    { return myTSegments; }

    typedef std::array<std::array<double, 2>, 2> ConditionType;

    CSplineCurve (const std::vector <CPnt>& theControlPoints);

    //With the boundary conditions.
    CSplineCurve (const std::vector <CPnt>& theControlPoints, const ConditionType& theConditions);

    //It creates a spline out of coefficients.
    CSplineCurve (const std::vector <std::array <double, 4>>& theXCoefficients, const std::vector <std::array <double, 4>>& theYCoefficients,
        const std::vector <double>& theDeltaT);

    CPnt Value (double t) const override;

    //It returns the first derivative's coordinates.
    Vec FirstDerivative (double t) const override;

    //It accepts a visitor.
    void Accept (Visitor& theVisitor) const override;

    //It returns the second derivative's coordinates.
    Vec SecondDerivative (double t) const override;

private:

    //It returns an index of coefficients for current t.
    size_t GetIndex (double t, double& theTofSegment) const;
};

#endif
