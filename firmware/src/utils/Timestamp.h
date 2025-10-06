#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <time.h>

#include <Arduino.h>

#include "config.h"

class Timestamp
{
public:
    static void initialize();
    static time_t getCurrentTime();
    static String formatTime(time_t epochTime);
    static String getFormattedCurrentTime();
};

#endif