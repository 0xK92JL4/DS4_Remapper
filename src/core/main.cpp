/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  main.cpp                                               ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/17 00:59:29 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/29 11:54:54 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "engine/EventLoop.hpp"

#include <iostream>
#include <cstdlib>
#include <csignal>

static void OnTerminate(int)
{
	EventLoop::RequestStop();
}

int main(void)
{
	signal(SIGCHLD, SIG_IGN);

	struct sigaction sa{};
	sa.sa_handler = OnTerminate;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT,  &sa, nullptr);
	sigaction(SIGTERM, &sa, nullptr);

	try
	{
		EventLoop loop;
		loop.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
