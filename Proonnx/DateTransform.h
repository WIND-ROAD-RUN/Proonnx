#ifndef DATETRANSFORM_H_
#define DATETRANSFORM_H_

#include<string>
#include<QString>

struct DateTransFormUtilty
{
    using RWstring = std::string;
    /*date from ISO to gregorian*/
    static RWstring dateFromISOToGregorian(const RWstring& date);
    /*date from gregorian to ISO*/
    static RWstring dateFromGregorianToISO(const RWstring& date);
    /*date remove -*/
    static RWstring removeHyphens(const RWstring& date);
    /*date remove /*/
    static RWstring removeSlash(const RWstring& date);
    /*date replace - with / */
    static RWstring replaceDashWithSlash(const RWstring& date);
    /*date replace / with - */
    static RWstring replaceSlashWithDash(const RWstring& date);

    static double dateHashSimilarity(const RWstring& input, const RWstring& target);

    static QString removeSymbolsAndSpaces(const QString& input);
};


#endif
