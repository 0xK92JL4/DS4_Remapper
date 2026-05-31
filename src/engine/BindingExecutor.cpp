/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  BindingExecutor.cpp                                    ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/31 00:29:52 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 23:07:43 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "engine/BindingExecutor.hpp"

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void BindingExecutor::Send(const BindingTarget& target, int value,
		VirtualMouse& mouse, VirtualKeyboard& keyboard)
{
	switch (target.type)
	{
		case ActionType::KeyboardKey:
			keyboard.SendKey(target.code, value);
			break;

		case ActionType::MouseButton:
			mouse.SendButton(target.code, value);
			break;

		default:
			break;
	}
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                 Public                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

void BindingExecutor::Press(const Action& action,
		VirtualMouse& mouse, VirtualKeyboard& keyboard)
{
    for (const auto& target : action.binding_keys)
        Send(target, Input::PRESS, mouse, keyboard);
}

void BindingExecutor::Release(const Action& action,
		VirtualMouse& mouse, VirtualKeyboard& keyboard)
{
    for (const auto& target : action.binding_keys)
        Send(target, Input::RELEASE, mouse, keyboard);
}
