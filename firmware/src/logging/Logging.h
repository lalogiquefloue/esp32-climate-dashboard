#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>
#include <string>
#include <vector>
#include "../config.h"

enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

class Logging {
    public:
        static void begin(unsigned long baud = BAUD);
        
        static void log(LogLevel level, const std::string& msg);
        static void Info(const std::string& msg);
        static void Warn(const std::string& msg);
        static void Error(const std::string& msg);
        static const std::vector<std::string>& getLogs();
    
    private:
        static std::vector<std::string> logs;
        static void addToBuffer(const std::string& msg);
};

#endif