#ifndef CFLG_LOGMODULE_H_
#define CFLG_LOGMODULE_H_

#include<string>

namespace rw {
    namespace cfglg {
        class LogModule {
        public:
            LogModule();

            ~LogModule();

        public:
            void iniCom();

            void desCom();

        public:
            void info(const std::string& str);

            void error(const std::string& str);

            void warn(const std::string& str);

        };
    
    }
}



#endif // !CFLG_LOGMODULE_H_
