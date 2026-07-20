#ifndef OVERVIEW_UTIL_H
#define OVERVIEW_UTIL_H

#include "gui_utility.h"

extern const struct gui_utility overview_utility;
void overview_set_rh(double rh);
void overview_set_press(double press);
void overview_set_temp(double temp);

#endif