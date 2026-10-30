/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceFinder.hpp                                       ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/10/29 12:00:00 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/29 12:00:00 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/


#pragma once

#include "core/structs.hpp"
#include <string>

class DeviceFinder
{
	public:
		DeviceFinder() = delete;

		/*
		 * Locates a connected DualShock 4 and every node we need:
		 *  - gamepad + touchpad evdev nodes (matched on vendor/product/name,
		 *    paired through the controller's `uniq`, i.e. its MAC)
		 *  - LED and battery sysfs nodes, resolved through the HID device
		 *    that owns the controller, so no node numbering is assumed.
		 *
		 * `mac_filter`: only accept this controller (case-insensitive,
		 * "aa:bb:cc:dd:ee:ff"). Empty = first controller found.
		 *
		 * Throws std::runtime_error with a readable reason on failure.
		 */
		static Ds4Paths Find(const std::string& mac_filter = "");
};
