#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <time.h>
#include <Arduino.h>
#include "config.h"

class Timestamp {
public:
    static void Initialize();
    static time_t GetCurrentTime();
    static String FormatTime(time_t epochTime);
    static String GetFormattedCurrentTime();
};

#endif