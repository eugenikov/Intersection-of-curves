#ifndef DATP_SPLINEDRIVER_H
#define DATP_SPLINEDRIVER_H

#include "DATP_BaseDriver.h"

class CCurve;

class DATP_SplineDriver : public DATP_BaseDriver {
public:
    virtual std::shared_ptr <CCurve> Parse (std::ifstream& theStream) override;
};
#endif
