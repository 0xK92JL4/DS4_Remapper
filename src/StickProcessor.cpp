/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  StickProcessor.cpp                                     ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:57:52 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/20 23:39:01 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "StickProcessor.hpp"
#include "Config.hpp"

#include <cmath>

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                         Constructor/Destructor                            │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

StickProcessor::StickProcessor(float sensivity_x, float sensivity_y)
	: _sens_x(sensivity_x)
	, _sens_y(sensivity_y) {}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Private                                    │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

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
		/ (Config::HALF_AXIS_RANGE - Config::DEADZONE);

	accumulator += push_ratio * sensitivity * dt;

	int delta = static_cast<int>(accumulator);

	accumulator -= delta;

	return delta;
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                Public                                     │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

Vec2 StickProcessor::Process(const Vec2& input, float dt)
{
	Vec2 out;

	out.x = ProcessAxis(input.x - Config::HALF_AXIS_RANGE, _acc_x, _sens_x, dt);
	out.y = ProcessAxis(input.y - Config::HALF_AXIS_RANGE, _acc_y, _sens_y, dt);

	return out;
}
