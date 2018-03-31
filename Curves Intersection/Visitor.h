#ifndef VISITOR_H
#define VISITOR_H

class CCurve;
class CSplineCurve;

class Visitor {
public:

    virtual void Visit (const CSplineCurve& theCurve) = 0;
};

#endif