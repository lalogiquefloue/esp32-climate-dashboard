#ifndef LOGGING_H
#define LOGGING_H

#include <vector>

#include <Arduino.h>

#include "config.h"
#include "utils/Timestamp.h"

enum LogLevel
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

class Logging
{
public:
    static void begin(unsigned long baud = BAUD);

    static void log(LogLevel level, const String &msg);
    static void info(const String &msg);
    static void warn(const String &msg);
    static void error(const String &msg);
    static const std::vector<String> &getLogs();

private:
    static std::vector<String> logs;
    static void addToBuffer(const String &msg);
};

#endif