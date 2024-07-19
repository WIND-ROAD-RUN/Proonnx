//#ifndef PUTILTY_H_
//#define UTILTY_H_
//
//bool check_is_date(const OCRResult& s);
//bool check_product(std::vector<OCRResult> product, QString standard);
//void printInfo();
//QString deleteStr(const QString& s, int num);
//bool isDateHead(const QString& s, const QString& prex);
//
//QString deleteStr(const QString& s, int num)
//{
//    auto result = s.toStdString();
//    if (num == 0 || result.empty()) {
//        return QString::fromStdString(result);
//    }
//    result = s.toStdString();
//    result = result.substr(num);
//
//    return QString::fromStdString(result);
//}
//bool isDateHead(const QString& s, const QString& prex)
//{
//    auto str = s.toStdString();
//
//    std::string prefix = prex.toStdString();
//
//    if (str.length() >= prefix.length()) {
//        return str.substr(0, prefix.length()) == prefix;
//    }
//    else {
//        return false;
//    }
//}
//std::string remove_num_chars(const std::string& input_string, int num_chars_to_remove) {
//    if (num_chars_to_remove < 0) {
//        std::cerr << "Error: num_chars_to_remove must be a non-negative integer." << std::endl;
//        return "";
//    }
//
//    if (num_chars_to_remove == 0 || input_string.empty()) {
//        return input_string;
//    }
//
//    return input_string.substr(num_chars_to_remove);
//}
//
//
//#endif