#include <string>

#include "DATFormatter.h"
#include "CCurve.h"
#include "FormatVisitor.h"

void DATFormatter::AddEntity (const std::shared_ptr <CCurve>& theEntity)
{
    myEntities.emplace (std::make_pair (theEntity, 1));
}

bool DATFormatter::Format (const std::string& theFileName)
{
    std::ofstream aStream;
    aStream.open (theFileName);

    if (!aStream.is_open()) {
        return false;
    }

    FormatVisitor aVisitor (aStream);

    for (const auto& it : myEntities) {
        it.first->Accept (aVisitor);
    }
    return true;
}