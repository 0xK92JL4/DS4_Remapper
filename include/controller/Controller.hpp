/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Controller.hpp                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/19 20:20:08 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:50:13 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "controller/LightBar.hpp"
#include "controller/Battery.hpp"
#include "engine/StickProcessor.hpp"
#include "input/InputDevice.hpp"
#include "output/VirtualKeyboard.hpp"
#include "output/VirtualMouse.hpp"
#include "core/structs.hpp"

#include <linux/input.h>

#include <optional>
#include <unordered_set>

class Controller
{
	private:
		std::optional<LightBar>	_lightbar;
		std::optional<Battery>	_battery;
		InputDevice	_ds4;
		InputDevice	_touchpad;

		StickProcessor  _mouse_stick;
		StickProcessor  _scroll_stick;

		int _axis_lx = 127;
		int _axis_ly = 127;
		int _axis_rx = 127;
		int _axis_ry = 127;

		Vec2 _move   = {0,0};
		Vec2 _scroll = {0,0};

		std::unordered_set<int> _pressed;

		void ExecuteAction(const Action&, int value, VirtualMouse&, VirtualKeyboard&);
		void HandleKeyEvent(const input_event&, VirtualMouse&, VirtualKeyboard&);
		void HandleAbsEvent(const input_event& ev);

	public:
		explicit Controller(const Ds4Paths& paths);
		~Controller() = default;

		Controller(const Controller&) = delete;
		Controller& operator=(const Controller&) = delete;

		InputDevice* GetDs4Device();
		InputDevice* GetTouchpadDevice();

		void	HandleDeviceEvent(InputDevice*, VirtualMouse&, VirtualKeyboard&);
		void	Update(float dt);

		void    UpdateLightBar();
		void    UpdateBattery();

		Vec2	GetMove() const;
		Vec2	GetScroll() const;

		bool	HasActiveMovement() const;

		void	ReleaseAll(VirtualMouse&, VirtualKeyboard&);

		bool			HasBattery() const;
		int				GetBatteryCapacity() const;   // -1 if unknown
		BatteryStatus	GetBatteryStatus() const;
};
