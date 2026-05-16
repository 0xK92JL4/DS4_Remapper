#pragma once
#include <unordered_map>
#include <linux/input.h>

namespace Config
{
    constexpr int DEADZONE = 20;
    constexpr int MAX_AXIS_RANGE = 127;
    constexpr float MOUSE_SENSITIVITY = 2.0f;
    
    constexpr int MAX_INTERVAL_MS = 250;
    constexpr int MIN_INTERVAL_MS = 35;

    const char* const DS4_DEVICE = "/dev/input/event21";
    const char* const TOUCHPAD_DEVICE = "/dev/input/event23";

    const std::unordered_map<int, int> ButtonMap = {
        { BTN_SOUTH,  BTN_LEFT },   // DS4 Cross      -> Mouse Left Click
        { BTN_EAST,   BTN_RIGHT },  // DS4 Circle     -> Mouse Right Click
        { BTN_NORTH,  BTN_MIDDLE }, // DS4 Triangle   -> Mouse Middle Click
    };
}
