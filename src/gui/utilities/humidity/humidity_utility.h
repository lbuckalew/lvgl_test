#ifndef HUMIDITY_UTILITY_H
#define HUMIDITY_UTILITY_H

#include "gui_utility.h"

extern const struct gui_utility humidity_utility;
void humidity_update(double relative_humidity);

#endif