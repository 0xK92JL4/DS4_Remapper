/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Config.hpp                                             ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:04 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/18 23:27:28 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <unordered_map>
#include <linux/input.h>

namespace Config
{
    constexpr int DEADZONE = 20;
    constexpr int HALF_AXIS_RANGE = 127;

    constexpr float MOUSE_SENS_X = 1.0f;
    constexpr float MOUSE_SENS_Y = 1.0f;

    constexpr float SCROLL_SENS_X = 0.035f; 
    constexpr float SCROLL_SENS_Y = 0.020f; 
 
	// grep -H . /sys/class/input/event*/device/name
    const char* const DS4_DEVICE = "/dev/input/event21";
    const char* const TOUCHPAD_DEVICE = "/dev/input/event23";

    const std::unordered_map<int, int> ButtonMap = {
        { BTN_SOUTH,  BTN_LEFT },
        { BTN_EAST,   BTN_RIGHT },
        { BTN_NORTH,  BTN_MIDDLE },
    };
}
