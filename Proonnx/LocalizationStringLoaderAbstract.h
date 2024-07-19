#ifndef LOCALIZATIONSTRINGLOADERABSTRACT_H_
#define LOCALIZATIONSTRINGLOADERABSTRACT_H_

#include<string>

class LocalizationStringLoaderAbstract {
public:
    virtual bool loadData() = 0;
    virtual const std::string& getString(const std::string id) = 0;
};


#endif // !LOCALIZATIONSTRINGLOADER_H_

