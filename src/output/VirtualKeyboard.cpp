/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  VirtualKeyboard.cpp                                    ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/29 18:47:23 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:29:54 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "output/VirtualKeyboard.hpp"
#include "core/Config.hpp"

#include <stdexcept>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

VirtualKeyboard::VirtualKeyboard()
{
	struct libevdev* vdev = libevdev_new();

	libevdev_set_name(vdev, "Virtual-Engine-Keyboard");

	libevdev_enable_event_type(vdev, EV_KEY);

	for (const auto& [input, action] : Config::InputMap)
	{
		if (action.type == ActionType::KeyboardKey)
		{
			libevdev_enable_event_code(vdev, EV_KEY, action.code, nullptr);
		}
		else if (action.type == ActionType::Binding)
		{
			for (const auto& b : action.binding_keys)
			{
				if (b.type == ActionType::KeyboardKey)
				{
					libevdev_enable_event_code(vdev, EV_KEY, b.code, nullptr);
				}
			}
		}
	}

	int ok = libevdev_uinput_create_from_device(
				vdev, LIBEVDEV_UINPUT_OPEN_MANAGED, &_uidev);

	libevdev_free(vdev);

	if (ok < 0)
		throw std::runtime_error("Failed to create virtual keyboard");
}

VirtualKeyboard::~VirtualKeyboard()
{
	if (_uidev)
		libevdev_uinput_destroy(_uidev);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void VirtualKeyboard::Emit(unsigned int type, unsigned int code, int value)
{
	libevdev_uinput_write_event(_uidev, type, code, value);
	libevdev_uinput_write_event(_uidev, EV_SYN, SYN_REPORT, 0);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Public                                     │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void VirtualKeyboard::SendKey(int virtual_key_code, int value)
{
	Emit(EV_KEY, virtual_key_code, value);
}

