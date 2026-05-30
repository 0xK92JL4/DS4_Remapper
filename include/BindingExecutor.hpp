/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  BindingExecutor.hpp                                    ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/31 00:26:31 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 00:53:27 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "structs.hpp"
#include "VirtualMouse.hpp"
#include "VirtualKeyboard.hpp"

#pragma once

class BindingExecutor
{
	private:
		static void Send(const BindingTarget&, int value, VirtualMouse&, VirtualKeyboard&);

	public:
		BindingExecutor() = delete;

		static void Press(const Action&, VirtualMouse&, VirtualKeyboard&);
		static void Release(const Action&, VirtualMouse&, VirtualKeyboard&);
};
