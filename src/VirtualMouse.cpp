/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  VirtualMouse.cpp                                       ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:59:17 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 01:20:59 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "VirtualMouse.hpp"
#include "Config.hpp"

#include <cmath>
#include <stdexcept>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

VirtualMouse::VirtualMouse()
{
    struct libevdev* vdev = libevdev_new();
    libevdev_set_name(vdev, "Virtual-Engine-Mouse");
    
    libevdev_enable_event_type(vdev, EV_REL);
    libevdev_enable_event_code(vdev, EV_REL, REL_X, nullptr);
    libevdev_enable_event_code(vdev, EV_REL, REL_Y, nullptr);
    libevdev_enable_event_code(vdev, EV_REL, REL_WHEEL, nullptr);
    libevdev_enable_event_code(vdev, EV_REL, REL_HWHEEL, nullptr);
    
	libevdev_enable_event_type(vdev, EV_KEY);

	for (const auto& [input, action] : Config::InputMap)
	{
		if (action.type == ActionType::MouseButton)
		{
			libevdev_enable_event_code(vdev, EV_KEY, action.code, nullptr);
		}
		else if (action.type == ActionType::Binding)
		{
			for (const auto& b : action.binding_keys)
			{
				if (b.type == ActionType::MouseButton)
				{
					libevdev_enable_event_code(vdev, EV_KEY, b.code, nullptr);
				}
			}
		}
	}

    int ok = libevdev_uinput_create_from_device(
				vdev, LIBEVDEV_UINPUT_OPEN_MANAGED, &_uidev);

    libevdev_free(vdev);

    if (ok < 0) throw std::runtime_error("Failed to create uinput device context.");
}

VirtualMouse::~VirtualMouse()
{
    if (_uidev) libevdev_uinput_destroy(_uidev);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void VirtualMouse::Emit(unsigned int type, unsigned int code, int value)
{
    libevdev_uinput_write_event(_uidev, type, code, value);
    libevdev_uinput_write_event(_uidev, EV_SYN, SYN_REPORT, 0);
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Public                                     │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void VirtualMouse::SendButton(int virtual_button_code, int value)
{
    Emit(EV_KEY, virtual_button_code, value);
}

void VirtualMouse::Move(Vec2 move)
{
	if (move.x != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_X, move.x);
	if (move.y != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_Y, move.y);

	if (move.x != 0 || move.y != 0)
		libevdev_uinput_write_event(_uidev, EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::Scroll(Vec2 scroll)
{
	if (scroll.x != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_HWHEEL, scroll.x);
	if (scroll.y != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_WHEEL, -scroll.y);

	if (scroll.x != 0 || scroll.y != 0)
		libevdev_uinput_write_event(_uidev, EV_SYN, SYN_REPORT, 0);
}
