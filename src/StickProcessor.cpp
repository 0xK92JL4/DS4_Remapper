/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  StickProcessor.cpp                                     ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:57:52 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/17 00:57:56 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "StickProcessor.hpp"
#include "Config.hpp"

#include <cmath>

int StickProcessor::ProcessAxis(
	int raw,
	float& accumulator,
	float sensitivity,
	float dt
)
{
	if (std::abs(raw) <= Config::DEADZONE)
	{
		accumulator = 0.0f;
		return 0;
	}

	float push_ratio = static_cast<float>(raw - (raw > 0 ? Config::DEADZONE : -Config::DEADZONE))
		/ (Config::MAX_AXIS_RANGE - Config::DEADZONE);

	accumulator += push_ratio * sensitivity * dt;

	int delta = static_cast<int>(accumulator);

	accumulator -= delta;

	return delta;
}

Vec2 StickProcessor::Process(
	int raw_x,
	int raw_y,
	float sens_x,
	float sens_y,
	float dt
)
{
	Vec2 out;

	out.x = ProcessAxis(raw_x, _acc_x, sens_x, dt);
	out.y = ProcessAxis(raw_y, _acc_y, sens_y, dt);

	return out;
}
