#include <fstream>
#include <string>
#include <vector>

#include "Errors.h"
#include "Helper.h"
#include "CPnt.h"

std::string Helper::GetFormat (const std::string& theSource)
{
    size_t aFound = theSource.find_last_of (".");
    if (!aFound) { return std::string(); }

    return theSource.substr (aFound);
}

std::string Helper::GetNameWithoutFormat (const std::string& theSource)
{
    size_t n = theSource.length();
    auto aNewString = theSource;
    return aNewString.erase (n - 4, 4);
}

std::vector <CPnt> Helper::ParsePoints (const std::string& theFileName)
{
    std::ifstream aStream;
    aStream.open (theFileName);
    std::vector <CPnt> aTarget;
    if (aStream.is_open()) {
        while (!aStream.eof()) {
            double aX;
            double aY;
            aStream >> aX;
            if (aStream.eof ()) {
                throw StreamError();
            }
            aStream >> aY;
            aTarget.push_back ({aX, aY});
        }
    } else {
        throw StreamError();
    }
    return aTarget;
}