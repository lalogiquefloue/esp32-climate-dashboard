#include "Timestamp.h"

void Timestamp::initialize()
{
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    // Set timezone
    setenv("TZ", TIME_ZONE_SPECIFICATION, 1);
    tzset();

    struct tm timeinfo;
    while (!getLocalTime(&timeinfo))
    {
        Serial.println("Waiting for NTP time sync...");
        delay(500);
    }

    Serial.println(&timeinfo, "Local time synced: %Y-%m-%d %H:%M:%S");
}

time_t Timestamp::getCurrentTime()
{
    return time(nullptr);
}

String Timestamp::formatTime(time_t time)
{
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time));
    return String(buffer);
}

String Timestamp::getFormattedCurrentTime()
{
    time_t now = getCurrentTime();
    if (now > 0)
    {
        return formatTime(now);
    }
    else
    {
        return "Invalid Time";
    }
}