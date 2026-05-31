/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  StickProcessor.hpp                                     ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:39 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/31 02:26:13 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "core/structs.hpp"

class StickProcessor
{
	private:
		float _acc_x = 0.0f;
		float _acc_y = 0.0f;

		float _sens_x = 1.0f;
		float _sens_y = 1.0f;

		int ProcessAxis(int raw, float& accumulator, float sensitivity, float dt);

	public:
		StickProcessor(float sensivity_x, float sensivity_y);

		Vec2 Process(const Vec2& input, float dt);
};

