#include "Logging.h"

std::vector<String> Logging::logs;

void Logging::begin(unsigned long baud)
{
    Serial.begin(baud);
    Serial.println("=== Logging started ===");
    Serial.print("Baud rate: ");
    Serial.println(baud);
}

void Logging::log(LogLevel level, const String &msg)
{
    String color;
    String prefix;
    String date_time = Timestamp::getFormattedCurrentTime();

    switch (level)
    {
    case LOG_INFO:
        prefix = "[INFO]  ";
        break;
    case LOG_WARNING:
        prefix = "[WARN]  ";
        break;
    case LOG_ERROR:
        prefix = "[ERROR] ";
        break;
    }

    const String formatted_msg = "[" + date_time + "] " + prefix + msg;

    addToBuffer(formatted_msg);
    Serial.println(formatted_msg.c_str());
}

void Logging::info(const String &msg) { log(LOG_INFO, msg); }
void Logging::warn(const String &msg) { log(LOG_WARNING, msg); }
void Logging::error(const String &msg) { log(LOG_ERROR, msg); }

const std::vector<String> &Logging::getLogs()
{
    return logs;
}

void Logging::addToBuffer(const String &msg)
{
    logs.push_back(msg);
    if (logs.size() > MAX_LOGS)
    {
        logs.erase(logs.begin());
    }
}