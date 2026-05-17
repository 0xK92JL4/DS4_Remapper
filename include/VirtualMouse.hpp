/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  VirtualMouse.hpp                                       ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:46 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 13:40:28 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <libevdev/libevdev-uinput.h>

class VirtualMouse
{
	public:
		VirtualMouse();
		~VirtualMouse();

		VirtualMouse(const VirtualMouse&) = delete;
		VirtualMouse& operator=(const VirtualMouse&) = delete;

		void Move(int dx, int dy);
		void Scroll(int rx, int ry);
		void SendButton(int virtual_button_code, int value);

	private:
		struct libevdev_uinput* uidev = nullptr;

		void Emit(unsigned int type, unsigned int code, int value);
};
