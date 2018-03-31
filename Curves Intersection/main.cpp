#include <memory>
#include <iostream>
#include <string>

#include "Errors.h"
#include "CSplineCurve.h"
#include "CPnt.h"
#include "Helper.h"
#include "Geom_Algo.h"
#include "DATFormatter.h"
#include "DATParser.h"

int main()
{
    std::shared_ptr <CCurve> aCurves[2];

    for (size_t i = 0; i < 2; ++i) {
        bool aCurveIsReady = false;
        while (!aCurveIsReady) {
            try {
                std::cout << "Input the " << (i == 0 ? "first" : "second") << " curve:" << std::endl;
                std::string aFileName;
                std::cin >> aFileName;

                auto ParseDAT = [&] (const std::string& theFileName) {
                    DATParser aParser;
                    aParser.Parse (theFileName);
                    const auto& aResVec = aParser.Result();
                    if (aResVec.size()) {
                        aCurves[i] = aResVec[0];
                        aCurveIsReady = true;
                    } else {
                        std::cout << "Empty dat file." << std::endl;
                    }
                };

                auto aFormatName = Helper::GetFormat (aFileName);
                if (aFormatName == ".txt") {
                    try {
                        const auto& aPntVec = Helper::ParsePoints (aFileName);
                        DATFormatter aFormatter;
                        aFormatter.AddEntity (std::make_shared <CSplineCurve> (aPntVec));
                        const auto& aNewName = Helper::GetNameWithoutFormat (aFileName) + ".dat";

                        std::cout << "Writing .dat file into " << aNewName << " ..." << std::endl;
                        aFormatter.Format (aNewName);
                        std::cout << "Parsing .dat file back..." << std::endl;

                        ParseDAT (aNewName);
                    } catch (GenerateError) {
                        std::cout << "Invalid data." << std::endl;
                    } catch (...) {
                        std::cout << "Error during transfer the data. Wrong file." << std::endl;
                    }
                } else if (aFormatName == ".dat") {
                        std::cout << "Parsing .dat file..." << std::endl;
                        ParseDAT (aFileName);
                } else {
                    std::cout << "Wrong file name or path." << std::endl;
                }
            } catch (ParserError) {
                std::cout << "Error during parsing the file." << std::endl;
            } catch (...) {
                std::cout << "Incorrect input." << std::endl;
            }
        }
    }

    std::vector <CPnt> anIntersectionsVec;
    Geom_Algo::Intersection (*aCurves[0], *aCurves[1], anIntersectionsVec, 1e-10);

    system ("pause");
    return 0;
}