#ifndef CCurve_H
#define CCurve_H

class CPnt;
class Visitor;

//CCurve is a base abstract class for all curves.
class CCurve
{
public:
    typedef CPnt Vec;

    //It returns coordinates of curve c(t).
    virtual CPnt Value (double t) const = 0;

    //It returns coordinates of first derivative.
    virtual Vec FirstDerivative (double t) const = 0;

    //It returns coordinates of second derivative.
    virtual Vec SecondDerivative (double t) const = 0;

    //It accepts a visitor.
    virtual void Accept (Visitor& theVisitor) const = 0;

    virtual ~CCurve() {};

    const double GetRange() const { return myCoefOfParameterization; }

protected:
    double myCoefOfParameterization;
};

#endif
