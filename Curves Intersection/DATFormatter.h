#ifndef DATFORMATTER_H
#define DATFORMATTER_H

#include <unordered_map>
#include <memory>

#include "CCurve.h"
#include "DATP_BaseDriver.h"

class DATFormatter
{
public:
    typedef std::unordered_map <std::shared_ptr <CCurve>, size_t> SourceType;

    bool Format (const std::string& theFileName);

    void AddEntity (const std::shared_ptr <CCurve>& theEntity);

private:

    SourceType myEntities;
};
#endif
