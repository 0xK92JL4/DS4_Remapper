#pragma once
#include <libevdev/libevdev-uinput.h>

class VirtualMouse
{
	public:
		VirtualMouse();
		~VirtualMouse();

		VirtualMouse(const VirtualMouse&) = delete;
		VirtualMouse& operator=(const VirtualMouse&) = delete;

		void Move(int raw_dx, int raw_dy);
		void Scroll(int raw_rx, int raw_ry, long long current_time_ms);
		void SendButton(int virtual_button_code, int value);

	private:
		struct libevdev_uinput* uidev = nullptr;
		float accumulator_x = 0.0f;
		float accumulator_y = 0.0f;
		long long last_v_scroll_time = 0;
		long long last_h_scroll_time = 0;

		int CalculateScrollInterval(int deflection);
		void Emit(unsigned int type, unsigned int code, int value);
};
