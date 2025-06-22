#pragma once

#include <Arduino.h>

void initClock();
String getTimestamp();
String getTimeOnly(); // HH:MM:SS
String getDateOnly(); // DD.MM.YYYY

String getCurrentDate();
String getCurrentTime();