#pragma once
#include <unordered_map>
#include <linux/input.h>

namespace Config {
    constexpr int DEADZONE = 20;
    constexpr int MAX_AXIS_RANGE = 127;

    // Sensitivities calibrated for millisecond delta-time tracking
    constexpr float MOUSE_SENS_X = 1.0f;
    constexpr float MOUSE_SENS_Y = 1.0f;

    constexpr float SCROLL_SENS_X = 0.035f; 
    constexpr float SCROLL_SENS_Y = 0.020f; 

    const char* const DS4_DEVICE = "/dev/input/event21";
    const char* const TOUCHPAD_DEVICE = "/dev/input/event23";

    const std::unordered_map<int, int> ButtonMap = {
        { BTN_SOUTH,  BTN_LEFT },
        { BTN_EAST,   BTN_RIGHT },
        { BTN_NORTH,  BTN_MIDDLE },
    };
}
