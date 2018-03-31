#ifndef HELPER_H
#define HELPER_H

class CPnt;

class Helper {
public:
    static std::string GetFormat (const std::string& theSource);
    static std::string GetNameWithoutFormat (const std::string& theSource);
    static std::vector <CPnt> ParsePoints (const std::string& theFileName);
};
#endif
