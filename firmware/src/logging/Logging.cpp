#include "Logging.h"

std::vector<std::string> Logging::logs;

void Logging::begin(unsigned long baud) {
    Serial.begin(baud);
    Serial.println("=== Logging started ===");
    Serial.print("Baud rate: "); 
    Serial.println(baud);
}

void Logging::log(LogLevel level, const std::string& msg) {
    const char* color;
    const char* prefix;
    const char* date_time = "date_time"; // TODO

    switch(level) {
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
    
    const std::string formatted_msg = std::string("[") + date_time + "] " + prefix + msg;    

    addToBuffer(formatted_msg);
    Serial.println(formatted_msg.c_str());
}

void Logging::Info(const std::string& msg)  { log(LOG_INFO, msg); }
void Logging::Warn(const std::string& msg)  { log(LOG_WARNING, msg); }
void Logging::Error(const std::string& msg) { log(LOG_ERROR, msg); }

const std::vector<std::string>& Logging::getLogs() {
    return logs;
}

void Logging::addToBuffer(const std::string& msg) {
    logs.push_back(msg);
    if (logs.size() > MAX_LOGS) {
        logs.erase(logs.begin());
    }
}