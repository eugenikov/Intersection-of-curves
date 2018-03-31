#include <fstream>
#include <string>
#include <memory>

#include "DATParser.h"
#include "DATP_SplineDriver.h"

DATParser::DATParser()
{
    //Filling map of drivers. The left (key) is uniqe id or rank of entity.
    //The right is driver of entity.
    myDriversMap.emplace (std::make_pair (1, std::make_shared <DATP_SplineDriver> ()));
}

bool DATParser::Parse (const std::string& theFileName)
{
    std::ifstream aStream;
    aStream.open (theFileName);

    if (!aStream.is_open()) {
        return false;
    }

    while (!aStream.eof()) {
        std::string anID;
        aStream >> anID;

        if (anID [0] == '#') {
            anID.erase (0, 1);
            size_t aRank = std::atoi (anID.c_str());

            if (myDriversMap.count (aRank)) {
                auto& aDriver = myDriversMap.at (aRank);
                myResultVector.push_back (aDriver->Parse (aStream));
            }
        }
    }
    return true;
}
