#include <vector>
#include <memory>
#include <array>
#include <fstream>

#include "Errors.h"
#include "DATP_SplineDriver.h"
#include "CCurve.h"
#include "CSplineCurve.h"

std::shared_ptr <CCurve> DATP_SplineDriver::Parse (std::ifstream& theStream)
{
    std::vector <std::array <double, 4>> aXCoeff;
    std::vector <std::array <double, 4>> aYCoeff;
    std::vector <double> aDeltaT;

    size_t aCountOfSegments = 0;

    theStream >> aCountOfSegments;

    aXCoeff.reserve (aCountOfSegments);
    aYCoeff.reserve (aCountOfSegments);
    aDeltaT.reserve (aCountOfSegments);
    //Parse Delta:
    for (size_t i = 0; i < aCountOfSegments; ++i) {
        double aDT (0);
        theStream >> aDT;
        aDeltaT.push_back (aDT);
    }

    enum {A, B, C, D};

    for (size_t j = 0; j < 2; ++j) {
        for (size_t i = 0; i < aCountOfSegments; ++i) {
            auto& aTargetCoeff = j == 0 ? aXCoeff : aYCoeff;
            std::array <double, 4> aCoeff;
            theStream >> aCoeff[A];
            theStream >> aCoeff[B];
            theStream >> aCoeff[C];

            if (theStream.eof()) {
                throw ParserError();
            }
            theStream >> aCoeff[D];

            aTargetCoeff.push_back (aCoeff);
        }
    }

    return std::make_shared <CSplineCurve> (aXCoeff, aYCoeff, aDeltaT);
}