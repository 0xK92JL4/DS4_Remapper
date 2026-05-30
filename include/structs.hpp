/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  structs.hpp                                            ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/20 23:29:57 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/30 17:58:15 by 0xK92JL4                 ▒▒    ▒▒        │*/
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
	Macro
};

struct MacroTarget
{
	ActionType	type;
	int			code;
};

struct Action
{
	ActionType					type;
	int							code;
	std::vector<MacroTarget>	macro_keys;
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

	static Action Macro(std::initializer_list<Action> actions)
	{
		Action parent(ActionType::Macro, 0);
		parent.macro_keys.reserve(actions.size());

		for (const auto& act : actions)
			parent.macro_keys.push_back({act.type, act.code});

		return parent;
	}

	static Action Command(std::initializer_list<std::string> args)
	{
		Action a(ActionType::Command, 0);
		a.cmd_args = args;
		return a;
	}
};
