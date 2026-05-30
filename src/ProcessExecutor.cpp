/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  ProcessExecutor.cpp                                    ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/05/30 14:06:00 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/05/30 14:12:08 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

#include "ProcessExecutor.hpp"
#include <spawn.h>
#include <vector>
#include <string>

extern char** environ;

bool ProcessExecutor::Execute(const std::vector<std::string>& args)
{
	if (args.empty())
		return false;

	std::vector<char*> av;
	av.reserve(args.size() + 1);

	for (const auto& arg : args)
	{
		av.push_back(const_cast<char*>(arg.c_str()));
	}
	av.push_back(nullptr);

	pid_t pid;
	return (
		posix_spawn(&pid, av[0], nullptr, nullptr, av.data(), environ) == 0
	);
}
