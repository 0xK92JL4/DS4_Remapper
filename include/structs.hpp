/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  structs.hpp                                            ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/20 23:29:57 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 00:52:44 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include <vector>
#include <string>

struct Vec2
{
	int x;
	int y;
};

enum class ActionType
{
	MouseButton,
	KeyboardKey,
	Command,
	Binding
};

struct BindingTarget
{
	ActionType	type;
	int			code;
};

struct Action
{
	ActionType					type;
	int							code;
	std::vector<BindingTarget>	binding_keys;
	std::vector<std::string>	cmd_args;

	Action(ActionType t, int c)
		: type(t), code(c) {}

	static Action MouseButton(int btn_code)
	{
		return Action(ActionType::MouseButton, btn_code);
	}

	static Action KeyboardKey(int key_code)
	{
		return Action(ActionType::KeyboardKey, key_code);
	}

	static Action Binding(std::vector<BindingTarget> targets)
	{
		Action a(ActionType::Binding, 0);
		a.binding_keys = std::move(targets);
		return a;
	}

	static Action Command(std::vector<std::string> args)
	{
		Action a(ActionType::Command, 0);
		a.cmd_args = args;
		return a;
	}
};
