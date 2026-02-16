#ifndef LOGLIB_H
#define LOGLIB_H

#include <string>
#include <mutex>

enum LEVEL_IMPORTANCE { LOW, MIDDLE, HIGH };
#define COUNT_LEVEL_IMPORTANCE 3

class Logger {
    private:
        std::string loggerName;
        LEVEL_IMPORTANCE defaultLevelImportance = LOW;
        mutable std::mutex fileMutex;
        std::mutex levelMutex;
    public:
        Logger(char* argv[]);
        Logger(std::string logName, LEVEL_IMPORTANCE level);
        void addMessage(std::string message, std::string levelImportance);
        void changeLevelImportance(std::string newLevelValue);
        std::string getLoggerName() const;
        LEVEL_IMPORTANCE getLevelImportance() const;
        LEVEL_IMPORTANCE convertStringToLEVEL(std::string level);
        std::string convertLEVELToString(LEVEL_IMPORTANCE level) const;
};

#endif