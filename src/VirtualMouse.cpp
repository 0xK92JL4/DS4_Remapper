/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  VirtualMouse.cpp                                       ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:59:17 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/18 22:58:44 by 0xK92JL4                 ▒▒    ▒▒        │*/
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
    libevdev_enable_event_code(vdev, EV_KEY, BTN_LEFT, nullptr);
    libevdev_enable_event_code(vdev, EV_KEY, BTN_RIGHT, nullptr);
    libevdev_enable_event_code(vdev, EV_KEY, BTN_MIDDLE, nullptr);

    int rc = libevdev_uinput_create_from_device(vdev, LIBEVDEV_UINPUT_OPEN_MANAGED, &_uidev);
    libevdev_free(vdev);

    if (rc < 0) throw std::runtime_error("Failed to create uinput device context.");
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

void VirtualMouse::Move(int dx, int dy)
{
	if (dx != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_X, dx);
	if (dy != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_Y, dy);

	if (dx != 0 || dy != 0)
		libevdev_uinput_write_event(_uidev, EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::Scroll(int sx, int sy)
{
	if (sx != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_HWHEEL, sx);
	if (sy != 0) libevdev_uinput_write_event(_uidev, EV_REL, REL_WHEEL, -sy);

	if (sx != 0 || sy != 0)
		libevdev_uinput_write_event(_uidev, EV_SYN, SYN_REPORT, 0);
}
