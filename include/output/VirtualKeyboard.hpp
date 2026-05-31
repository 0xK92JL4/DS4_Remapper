/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  VirtualKeyboard.hpp                                    ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/29 18:43:48 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:26:29 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "core/structs.hpp"
#include <libevdev/libevdev-uinput.h>

class VirtualKeyboard
{
	private:
		struct libevdev_uinput* _uidev = nullptr;

		void Emit(unsigned int type, unsigned int code, int value);

	public:
		VirtualKeyboard();
		~VirtualKeyboard();

		VirtualKeyboard(const VirtualKeyboard&) = delete;
		VirtualKeyboard& operator=(const VirtualKeyboard&) = delete;

		void SendKey(int virtual_key_code, int value);
};
