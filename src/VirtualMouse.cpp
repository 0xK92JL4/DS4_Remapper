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

    if (rc < 0)
	{
        throw std::runtime_error("Failed to create uinput device context.");
    }
}

VirtualMouse::~VirtualMouse()
{
    if (uidev)
	{
        libevdev_uinput_destroy(uidev);
    }
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

void VirtualMouse::Move(int raw_dx, int raw_dy)
{
    int move_x = 0, move_y = 0;

    if (std::abs(raw_dx) > Config::DEADZONE)
	{
        float push_ratio = static_cast<float>(raw_dx - (raw_dx > 0 ? Config::DEADZONE : -Config::DEADZONE)) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
        accumulator_x += push_ratio * Config::MOUSE_SENSITIVITY;
    }
	else
	{
        accumulator_x = 0.0f;
    }

    if (std::abs(raw_dy) > Config::DEADZONE)
	{
        float push_ratio = static_cast<float>(raw_dy - (raw_dy > 0 ? Config::DEADZONE : -Config::DEADZONE)) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
        accumulator_y += push_ratio * Config::MOUSE_SENSITIVITY;
    }
	else
	{
        accumulator_y = 0.0f;
    }

    if (std::abs(static_cast<int>(accumulator_x)) >= 1) { move_x = static_cast<int>(accumulator_x); accumulator_x -= move_x; }
    if (std::abs(static_cast<int>(accumulator_y)) >= 1) { move_y = static_cast<int>(accumulator_y); accumulator_y -= move_y; }

    if (move_x != 0) libevdev_uinput_write_event(uidev, EV_REL, REL_X, move_x);
    if (move_y != 0) libevdev_uinput_write_event(uidev, EV_REL, REL_Y, move_y);
    if (move_x != 0 || move_y != 0)
	{
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    }
}

int VirtualMouse::CalculateScrollInterval(int deflection)
{
    int abs_deflect = std::abs(deflection);
    if (abs_deflect <= Config::DEADZONE) return Config::MAX_INTERVAL_MS;
    float push_ratio = static_cast<float>(abs_deflect - Config::DEADZONE) / (Config::MAX_AXIS_RANGE - Config::DEADZONE);
    if (push_ratio > 1.0f) push_ratio = 1.0f;
    return Config::MAX_INTERVAL_MS - static_cast<int>(push_ratio * (Config::MAX_INTERVAL_MS - Config::MIN_INTERVAL_MS));
}

void VirtualMouse::Scroll(int raw_rx, int raw_ry, long long current_time_ms)
{
    if (std::abs(raw_ry) > Config::DEADZONE)
	{
        int v_interval = CalculateScrollInterval(raw_ry);
        if (current_time_ms - last_v_scroll_time >= v_interval)
		{
            Emit(EV_REL, REL_WHEEL, (raw_ry > 0) ? -1 : 1);
            last_v_scroll_time = current_time_ms;
        }
    }
    if (std::abs(raw_rx) > Config::DEADZONE)
	{
        int h_interval = CalculateScrollInterval(raw_rx);
        if (current_time_ms - last_h_scroll_time >= h_interval)
		{
            Emit(EV_REL, REL_HWHEEL, (raw_rx > 0) ? 1 : -1);
            last_h_scroll_time = current_time_ms;
        }
    }
}
