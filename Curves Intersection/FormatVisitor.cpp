#include <array>

#include "FormatVisitor.h"
#include "CSplineCurve.h"

void FormatVisitor::Visit (const CSplineCurve& theCurve)
{
    const auto& aXCoef = theCurve.GetXCoef();
    const auto& aYCoef = theCurve.GetYCoef();
    const auto& aDT = theCurve.GetDT();

    myStream.precision (16);
    myStream << "#1" << std::endl;
    myStream << aDT.size() << std::endl;

    for (size_t i = 0; i < aDT.size(); ++i) {
        myStream << aDT[i] << std::endl;
    }

    enum {A, B, C, D};

    for (size_t j = 0; j < 2; ++j) {
        for (size_t i = 0; i < aDT.size(); ++i) {
            const auto& aCoef = j == 0 ? aXCoef : aYCoef;
            myStream << aCoef[i][A] << std::endl;
            myStream << aCoef[i][B] << std::endl;
            myStream << aCoef[i][C] << std::endl;
            myStream << aCoef[i][D] << std::endl;
        }
    }
}
