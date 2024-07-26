#pragma warning(disable:4996)

#include "DateTransform.h"

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include<sstream>
#include <algorithm>
#include <QRegExp>

DateTransFormUtilty::RWstring
DateTransFormUtilty::dateFromISOToGregorian
(const RWstring& data)
{
	std::tm tm = {};
	std::istringstream ss(data);
	ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

	std::time_t time = std::mktime(&tm);
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
	std::time_t time_t = std::chrono::system_clock::to_time_t(tp);
	std::tm* gmtm = std::gmtime(&time_t);

	char buffer[80];
	std::strftime(buffer, 80, "%Y-%m-%d", gmtm);

	return RWstring(buffer);
}

DateTransFormUtilty::RWstring
DateTransFormUtilty::dateFromGregorianToISO
(const RWstring& date)
{
	std::tm tm = {};
	std::istringstream ss(date);
	ss >> std::get_time(&tm, "%Y-%m-%d");

	std::time_t time = std::mktime(&tm);
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
	std::time_t time_t = std::chrono::system_clock::to_time_t(tp);
	std::tm* gmtm = std::gmtime(&time_t);

	std::ostringstream oss;
	oss << std::put_time(gmtm, "%Y-%m-%dT%H:%M:%S");

	return oss.str();
}

DateTransFormUtilty::RWstring
DateTransFormUtilty::removeHyphens(const RWstring& date)
{
	RWstring result = date;
	result.erase
	(std::remove(result.begin(), result.end(), '-')
		, result.end());
	return result;
}

DateTransFormUtilty::RWstring
DateTransFormUtilty::removeSlash(const RWstring& date)
{
	RWstring result = date;
	result.erase
	(std::remove(result.begin(), result.end()
		, '/')
		, result.end());
	return result;
}

DateTransFormUtilty::RWstring
DateTransFormUtilty::replaceDashWithSlash
(const RWstring& date)
{
	std::string result = date;
	std::replace
	(result.begin(), result.end()
		, '-'
		, '/');
	return result;
}

DateTransFormUtilty::RWstring
DateTransFormUtilty::replaceSlashWithDash
(const RWstring& date)
{
	RWstring result = date;
	std::replace
	(result.begin(), result.end()
		, '/'
		, '-');
	return result;
}

double 
DateTransFormUtilty::dateHashSimilarity
(const RWstring& input, const RWstring& target)
{

	RWstring inputLower = input;
	std::transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);

	RWstring targetLower = target;
	std::transform(targetLower.begin(), targetLower.end(), targetLower.begin(), ::tolower);

	int sameCharCount = 0;
	int totalCharCount = 0;

	for (size_t i = 0; i < inputLower.size(); ++i) {
		if (i < targetLower.size()) {
			if (inputLower[i] == targetLower[i]) {
				sameCharCount++;
			}
			totalCharCount++;
		}
		else {
			break;
		}
	}

	return static_cast<double>(sameCharCount) / totalCharCount;

}

QString DateTransFormUtilty::removeSymbolsAndSpaces(const QString& input)
{
	QString result = input;
	result.remove(QRegExp("[^a-zA-Z0-9]")); 
	return result;
}




