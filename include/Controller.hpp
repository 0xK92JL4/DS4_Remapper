/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.hpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/19 20:20:08 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/20 21:39:43 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "InputDevice.hpp"
#include "VirtualMouse.hpp"
#include <linux/input.h>

class Controller
{
	private:
		InputDevice _ds4;
		InputDevice _touchpad;

		int _axis_lx = 127;
		int _axis_ly = 127;
		int _axis_rx = 127;
		int _axis_ry = 127;

	public:
		Controller();
		~Controller() = default;

		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;

		InputDevice* GetDs4Device();
		InputDevice* GetTouchpadDevice();

		void	HandleDeviceEvent(InputDevice* device, VirtualMouse& virtual_mouse);

		int		GetLeftStickX() const;
		int		GetLeftStickY() const;
		int		GetRightStickX() const;
		int		GetRightStickY() const;

		bool	HasActiveMovement() const;
};
