/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Config.hpp                                             ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:04 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 10:27:44 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "core/structs.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <linux/input.h>
#include <cstdint>
#include <chrono>

namespace Config
{
    constexpr int DEADZONE = 20;
    constexpr int HALF_AXIS = 127;


    constexpr float MOUSE_SENS_X = 1.0f;
    constexpr float MOUSE_SENS_Y = 1.0f;

    constexpr float SCROLL_SENS_X = 0.035f; 
    constexpr float SCROLL_SENS_Y = 0.020f; 

    constexpr uint32_t COLOR = 0xAA0000;

	inline const std::vector<std::string> TOGGLE_SVKBD = {
		"/usr/bin/sh", "-c",

		"pgrep -x svkbd-en >/dev/null && pkill -x svkbd-en"
		" || svkbd-en -g 1246x356 >/dev/null 2>&1 &"
	};

	constexpr auto BATTERY_REFRESH_RATE = std::chrono::seconds(10);
	constexpr auto LED_REFRESH_RATE = std::chrono::milliseconds(50);
 
    inline const std::string DS4_MAC = "";

	inline constexpr auto MouseButton = Action::MouseButton;
    inline constexpr auto KeyboardKey = Action::KeyboardKey;
	inline constexpr auto Binding     = Action::Binding;
    inline constexpr auto Command     = Action::Command;

	inline const std::unordered_map<int, Action> InputMap = {
		{ BTN_SOUTH,   MouseButton(BTN_LEFT)       },
		{ BTN_EAST,    MouseButton(BTN_RIGHT)      },
		{ BTN_NORTH,   MouseButton(BTN_MIDDLE)     },

		{ BTN_START,   KeyboardKey(KEY_MUTE)       },
		{ BTN_TL,      KeyboardKey(KEY_VOLUMEDOWN) },
		{ BTN_TR,      KeyboardKey(KEY_VOLUMEUP)   },

		{ BTN_WEST,    Command(TOGGLE_SVKBD)       },
		{
			BTN_THUMBR,
			Binding({
				BindingTarget::KeyboardKey(KEY_LEFTALT),
				BindingTarget::MouseButton(BTN_LEFT)
			})
		},
	};
}
