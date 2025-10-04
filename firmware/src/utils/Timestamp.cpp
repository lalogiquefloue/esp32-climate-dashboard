#include "Timestamp.h"

void Timestamp::Initialize() {
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    // Set timezone
    setenv("TZ", TIME_ZONE_SPECIFICATION, 1);
    tzset();

    struct tm timeinfo;
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Waiting for NTP time sync...");
        delay(500);
    }

    Serial.println(&timeinfo, "Local time synced: %Y-%m-%d %H:%M:%S");
}

time_t Timestamp::GetCurrentTime() {
    return time(nullptr);
}

String Timestamp::FormatTime(time_t time) {
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time));
    return String(buffer);
}

String Timestamp::GetFormattedCurrentTime() {
    time_t now = GetCurrentTime();
    if (now > 0) {
        return FormatTime(now);
    } else {
        return "Invalid Time";
    }
}