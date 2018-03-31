#ifndef DATP_BASEDRIVER_H
#define DATP_BASEDRIVER_H
class CCurve;

class DATP_BaseDriver {
public:
    virtual std::shared_ptr <CCurve> Parse (std::ifstream& theStream) = 0;

    virtual ~DATP_BaseDriver() {};
};

#endif

