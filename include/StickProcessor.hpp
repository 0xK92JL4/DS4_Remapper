/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  StickProcessor.hpp                                     ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:58:39 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 00:58:40 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

struct Vec2
{
	int x;
	int y;
};

class StickProcessor
{
	public:
		Vec2 Process(int raw_x, int raw_y,float sens_x, float sens_y, float dt);

	private:
		float _acc_x = 0.0f;
		float _acc_y = 0.0f;

		int ProcessAxis(int raw, float& accumulator, float sensitivity, float dt);
};
