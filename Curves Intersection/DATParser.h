#ifndef DATPARSER_H
#define DATPARSER_H
#include <unordered_map>
#include <vector>

#include "DATP_BaseDriver.h"

class DATParser
{
public:

    typedef std::vector <std::shared_ptr <CCurve>>                         ResultType;
    typedef std::unordered_map <size_t, std::shared_ptr <DATP_BaseDriver>> DriversMapType;

    DATParser();

    bool Parse (const std::string& theFileName);

    const ResultType& Result() { return myResultVector; }

private:

    DriversMapType myDriversMap;
    ResultType myResultVector;

};
#endif
