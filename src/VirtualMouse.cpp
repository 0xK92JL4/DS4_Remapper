#include "VirtualMouse.hpp"
#include "Config.hpp"
#include <cmath>
#include <stdexcept>

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

    int rc = libevdev_uinput_create_from_device(vdev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    libevdev_free(vdev);

    if (rc < 0) throw std::runtime_error("Failed to create uinput device context.");
}

VirtualMouse::~VirtualMouse()
{
    if (uidev) libevdev_uinput_destroy(uidev);
}

void VirtualMouse::Emit(unsigned int type, unsigned int code, int value)
{
    libevdev_uinput_write_event(uidev, type, code, value);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
}

void VirtualMouse::SendButton(int virtual_button_code, int value)
{
    Emit(EV_KEY, virtual_button_code, value);
}

void VirtualMouse::Move(int raw_dx, int raw_dy, float dt)
{
    int move_x = 0, move_y = 0;

    if (std::abs(raw_dx) > Config::DEADZONE)
	{
        float push_ratio = static_cast<float>(raw_dx - (raw_dx > 0 ? Config::DEADZONE : -Config::DEADZONE)) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
        mouse_acc_x += push_ratio * Config::MOUSE_SENS_X * dt;
    }
	else
	{
        mouse_acc_x = 0.0f;
    }

    if (std::abs(raw_dy) > Config::DEADZONE)
	{
        float push_ratio = static_cast<float>(raw_dy - (raw_dy > 0 ? Config::DEADZONE : -Config::DEADZONE)) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
        mouse_acc_y += push_ratio * Config::MOUSE_SENS_Y * dt;
    }
	else
	{
        mouse_acc_y = 0.0f;
    }

    if (std::abs(static_cast<int>(mouse_acc_x)) >= 1) { move_x = static_cast<int>(mouse_acc_x); mouse_acc_x -= move_x; }
    if (std::abs(static_cast<int>(mouse_acc_y)) >= 1) { move_y = static_cast<int>(mouse_acc_y); mouse_acc_y -= move_y; }

    if (move_x != 0) libevdev_uinput_write_event(uidev, EV_REL, REL_X, move_x);
    if (move_y != 0) libevdev_uinput_write_event(uidev, EV_REL, REL_Y, move_y);
    if (move_x != 0 || move_y != 0)
	{
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }
}

void VirtualMouse::Scroll(int raw_rx, int raw_ry, float dt)
{
    int scroll_x = 0, scroll_y = 0;

    if (std::abs(raw_rx) > Config::DEADZONE)
	{
        float push_ratio = static_cast<float>(raw_rx - (raw_rx > 0 ? Config::DEADZONE : -Config::DEADZONE)) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
        scroll_acc_x += push_ratio * Config::SCROLL_SENS_X * dt;
    }
	else
	{
        scroll_acc_x = 0.0f;
    }

    if (std::abs(raw_ry) > Config::DEADZONE)
	{
        float push_ratio = static_cast<float>(raw_ry - (raw_ry > 0 ? Config::DEADZONE : -Config::DEADZONE)) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
        scroll_acc_y += push_ratio * Config::SCROLL_SENS_Y * dt;
    }
	else
	{
        scroll_acc_y = 0.0f;
    }

    if (std::abs(static_cast<int>(scroll_acc_x)) >= 1) { scroll_x = static_cast<int>(scroll_acc_x); scroll_acc_x -= scroll_x; }
    if (std::abs(static_cast<int>(scroll_acc_y)) >= 1) { scroll_y = static_cast<int>(scroll_acc_y); scroll_acc_y -= scroll_y; }

    if (scroll_x != 0) libevdev_uinput_write_event(uidev, EV_REL, REL_HWHEEL, scroll_x);
    if (scroll_y != 0) libevdev_uinput_write_event(uidev, EV_REL, REL_WHEEL, -scroll_y); 
    
    if (scroll_x != 0 || scroll_y != 0)
	{
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }
}
