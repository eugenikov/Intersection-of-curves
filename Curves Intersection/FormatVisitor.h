#ifndef FORMATVISITOR_H
#define FORMATVISITOR_H

#include <fstream>

#include "Visitor.h"

class FormatVisitor : public Visitor
{
public:
    FormatVisitor (std::ofstream& theStream) : myStream (theStream) {};

    void Visit (const CSplineCurve& theCurve) override;
private:

    std::ofstream& myStream;
};

#endif
