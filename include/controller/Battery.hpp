/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  Battery.hpp                                            ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/25 12:29:41 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/26 22:35:41 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#pragma once

#include "BatteryStatus.hpp"
#include <string>

class Battery
{
	private:
		int _fd_capacity = -1;
		int _fd_status = -1;

		int				_capacity;
		BatteryStatus	_status;

		std::string		Read(int fd);

		int				ParseCapacity(const std::string& s);
		BatteryStatus	ParseStatus(const std::string& s);

	public:
		Battery();
		~Battery();

		void			Update();

		int				GetCapacity() const;
		BatteryStatus	GetStatus() const;
};
