#include "loglib.h"
#include <fstream>
#include <iostream>
#include <iomanip>

Logger::Logger(char* logName[]) 
    : loggerName(logName[1]), defaultLevelImportance(convertStringToLEVEL(logName[2])) {}

Logger::Logger(std::string filename, LEVEL_IMPORTANCE defaultLevel) 
    : loggerName(filename), defaultLevelImportance(defaultLevel) {}

std::string Logger::getLoggerName() const {
    return loggerName;
}

LEVEL_IMPORTANCE Logger::getLevelImportance() const {
    return defaultLevelImportance;
}

LEVEL_IMPORTANCE Logger::convertStringToLEVEL(std::string level) {
    LEVEL_IMPORTANCE result = getLevelImportance();
    if (level == "HIGH")
        result = HIGH;
    else if (level == "MIDDLE")
        result = MIDDLE;
    else if (level == "LOW")
        result = LOW;
    else if (level != "NONE")
        std::cout << "Entered level of importance is uncorrect, default value will be used .\n";
    return result;
}

std::string Logger::convertLEVELToString(LEVEL_IMPORTANCE level) const {
    std::string result;
    switch (level) {
        case 0: result = "LOW"; break;
        case 1: result = "MIDDLE"; break;
        case 2: result = "HIGH"; break;
    }
    return result;
}

std::string getCurrentTime() {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

void Logger::addMessage(std::string message, std::string levelImportance) {
    LEVEL_IMPORTANCE level = convertStringToLEVEL(levelImportance);
    {
        std::lock_guard<std::mutex> levelLock(levelMutex);
        if (level < defaultLevelImportance) return;
    }
    std::lock_guard<std::mutex> fileLock(fileMutex);
    std::ofstream out(loggerName, std::ios::app);
    if (out.is_open()) {
        out << message << '\n' << convertLEVELToString(level) << '\n' << getCurrentTime();
    }
    out.close();        
}

void Logger::changeLevelImportance(std::string newLevelValue) {
    std::lock_guard<std::mutex> lock(levelMutex);
    defaultLevelImportance = convertStringToLEVEL(newLevelValue);
}