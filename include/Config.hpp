/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Config.hpp                                             ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:04 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/25 12:41:55 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <unordered_map>
#include <linux/input.h>
#include <cstdint>

namespace Config
{
    constexpr int DEADZONE = 20;
    constexpr int HALF_AXIS = 127;


    constexpr float MOUSE_SENS_X = 1.0f;
    constexpr float MOUSE_SENS_Y = 1.0f;

    constexpr float SCROLL_SENS_X = 0.035f; 
    constexpr float SCROLL_SENS_Y = 0.020f; 

    constexpr uint32_t COLOR = 0xAA0000;
 
	// grep -H . /sys/class/input/event*/device/name
    const char* const DS4_DEVICE = "/dev/input/event21";
    const char* const TOUCHPAD_DEVICE = "/dev/input/event23";

	const char* const LED_R = "/sys/class/leds/input22:red/brightness";
	const char* const LED_G = "/sys/class/leds/input22:green/brightness";
	const char* const LED_B = "/sys/class/leds/input22:blue/brightness";
	const char* const LED_GLOBAL = "/sys/class/leds/input22:global/brightness";

	const char* const BATTERY_CAPACITY
		= "/sys/class/power_supply/ps-controller-battery-28:c1:3c:48:43:83/capacity";
	const char* const BATTERY_STATUS
		= "/sys/class/power_supply/ps-controller-battery-28:c1:3c:48:43:83/status";

    const std::unordered_map<int, int> ButtonMap = {
        { BTN_SOUTH,  BTN_LEFT },
        { BTN_EAST,   BTN_RIGHT },
        { BTN_NORTH,  BTN_MIDDLE },
    };
}
