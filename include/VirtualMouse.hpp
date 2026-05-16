#pragma once
#include <libevdev/libevdev-uinput.h>

class VirtualMouse {
public:
    VirtualMouse();
    ~VirtualMouse();

    VirtualMouse(const VirtualMouse&) = delete;
    VirtualMouse& operator=(const VirtualMouse&) = delete;

    void Move(int raw_dx, int raw_dy, float dt);
    void Scroll(int raw_rx, int raw_ry, float dt);
    void SendButton(int virtual_button_code, int value);

private:
    struct libevdev_uinput* uidev = nullptr;
    
    float mouse_acc_x = 0.0f;
    float mouse_acc_y = 0.0f;
    float scroll_acc_x = 0.0f;
    float scroll_acc_y = 0.0f;

    void Emit(unsigned int type, unsigned int code, int value);
};
