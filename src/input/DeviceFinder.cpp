/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                                                           │*/
/*│  DeviceFinder.cpp                                       ▒▒▒▒    ▒▒▒▒      │*/
/*│                                                         ▒▒▒▒    ▒▒▒▒      │*/
/*│  By: 0xK92JL4                                               ▒▒▒▒          │*/
/*│                                                           ▒▒▒▒▒▒▒▒        │*/
/*│  Created: 2026/10/29 12:00:00 by 0xK92JL4                 ▒▒▒▒▒▒▒▒        │*/
/*│  Updated: 2026/10/30 15:54:48 by 0xK92JL4                 ▒▒    ▒▒        │*/
/*│                                                                           │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/


#include "input/DeviceFinder.hpp"

#include <libevdev/libevdev.h>

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

namespace fs = std::filesystem;

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                  Helpers                                  │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

namespace
{
	constexpr int SONY_VENDOR_ID = 0x054c;

	// DS4 v1, DS4 v2, Sony USB wireless adapter
	constexpr int DS4_PRODUCT_IDS[] = { 0x05c4, 0x09cc, 0x0ba0 };

	constexpr const char* GAMEPAD_NAME  = "Wireless Controller";
	constexpr const char* TOUCHPAD_NAME = "Wireless Controller Touchpad";

	constexpr const char* BATTERY_PREFIX = "ps-controller-battery";

	struct Candidate
	{
		std::string	path;
		std::string	uniq;
		bool		is_touchpad;
	};

	std::string Lower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(),
			[](unsigned char c) { return static_cast<char>(std::tolower(c)); });
		return s;
	}

	bool EndsWith(const std::string& s, const std::string& suffix)
	{
		return s.size() >= suffix.size()
			&& s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
	}

	bool StartsWith(const std::string& s, const std::string& prefix)
	{
		return s.compare(0, prefix.size(), prefix) == 0;
	}

	bool IsUnder(const std::string& path, const std::string& dir)
	{
		return path.size() > dir.size()
			&& path.compare(0, dir.size(), dir) == 0
			&& path[dir.size()] == '/';
	}

	bool LooksLikeHidId(const std::string& s)
	{
		if (s.size() != 19)
			return false;

		for (size_t i = 0; i < s.size(); i++)
		{
			if (i == 4 || i == 9)
			{
				if (s[i] != ':') return false;
			}
			else if (i == 14)
			{
				if (s[i] != '.') return false;
			}
			else if (!std::isxdigit(static_cast<unsigned char>(s[i])))
				return false;
		}
		return true;
	}

	std::vector<std::string> ListEventNodes()
	{
		std::vector<std::pair<int, std::string>> nodes;
		std::error_code ec;

		for (fs::directory_iterator it("/dev/input", ec), end;
			!ec && it != end; it.increment(ec))
		{
			const std::string name = it->path().filename().string();

			if (!StartsWith(name, "event") || name.size() == 5)
				continue;

			nodes.emplace_back(std::atoi(name.c_str() + 5), it->path().string());
		}

		std::sort(nodes.begin(), nodes.end());

		std::vector<std::string> paths;
		paths.reserve(nodes.size());
		for (auto& n : nodes)
			paths.push_back(std::move(n.second));

		return paths;
	}

	std::optional<Candidate> Probe(const std::string& path)
	{
		int fd = open(path.c_str(), O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd < 0)
			return std::nullopt;

		struct libevdev* dev = nullptr;
		if (libevdev_new_from_fd(fd, &dev) < 0)
		{
			close(fd);
			return std::nullopt;
		}

		const char* name = libevdev_get_name(dev);
		const char* uniq = libevdev_get_uniq(dev);
		const int vendor = libevdev_get_id_vendor(dev);
		const int product = libevdev_get_id_product(dev);

		std::optional<Candidate> result;

		const bool is_ds4 = vendor == SONY_VENDOR_ID
			&& std::find(std::begin(DS4_PRODUCT_IDS), std::end(DS4_PRODUCT_IDS),
				product) != std::end(DS4_PRODUCT_IDS);

		if (is_ds4 && name)
		{
			const std::string n = name;
			const std::string u = uniq ? Lower(uniq) : "";

			if (n == GAMEPAD_NAME)
				result = Candidate{path, u, false};
			else if (n == TOUCHPAD_NAME)
				result = Candidate{path, u, true};
		}

		libevdev_free(dev);
		close(fd);
		return result;
	}

	std::string HidDirOf(const std::string& event_path)
	{
		std::error_code ec;
		const std::string node = fs::path(event_path).filename().string();

		fs::path p = fs::canonical("/sys/class/input/" + node + "/device", ec);
		if (ec)
			return "";

		while (p.has_relative_path())
		{
			if (LooksLikeHidId(p.filename().string()))
				return p.string();
			p = p.parent_path();
		}
		return "";
	}

	void FindLeds(const std::string& hid_dir, Ds4Paths& out)
	{
		std::error_code ec;

		for (fs::directory_iterator it("/sys/class/leds", ec), end;
			!ec && it != end; it.increment(ec))
		{
			std::error_code cec;
			const fs::path real = fs::canonical(it->path(), cec);

			if (cec || !IsUnder(real.string(), hid_dir))
				continue;

			const std::string name = real.filename().string();
			const std::string node = (real / "brightness").string();

			if      (EndsWith(name, ":red"))    out.led_r = node;
			else if (EndsWith(name, ":green"))  out.led_g = node;
			else if (EndsWith(name, ":blue"))   out.led_b = node;
			else if (EndsWith(name, ":global")) out.led_global = node;
		}
	}

	void FindBattery(const std::string& hid_dir, Ds4Paths& out)
	{
		std::error_code ec;

		for (fs::directory_iterator it("/sys/class/power_supply", ec), end;
			!ec && it != end; it.increment(ec))
		{
			std::error_code cec;
			const fs::path real = fs::canonical(it->path(), cec);

			if (cec)
				continue;

			const std::string name = real.filename().string();

			const bool by_path = IsUnder(real.string(), hid_dir);
			const bool by_name = !out.uniq.empty()
				&& StartsWith(name, BATTERY_PREFIX)
				&& Lower(name).find(out.uniq) != std::string::npos;

			if (!by_path && !by_name)
				continue;

			out.battery_capacity = (real / "capacity").string();
			out.battery_status   = (real / "status").string();
			return;
		}
	}
}

/*┌───────────────────────────────────────────────────────────────────────────┐*/
/*│                                   Public                                  │*/
/*└───────────────────────────────────────────────────────────────────────────┘*/

Ds4Paths DeviceFinder::Find(const std::string& mac_filter, bool require_sysfs)
{
	const std::string wanted = Lower(mac_filter);

	std::vector<Candidate> found;
	for (const auto& path : ListEventNodes())
	{
		if (auto c = Probe(path))
			found.push_back(std::move(*c));
	}

	if (found.empty())
		throw std::runtime_error(
			"No DualShock 4 found in /dev/input (not connected, or no permission on /dev/input?)");

	for (const auto& pad : found)
	{
		if (pad.is_touchpad)
			continue;

		if (!wanted.empty() && pad.uniq != wanted)
			continue;

		auto touchpad = std::find_if(found.begin(), found.end(),
			[&](const Candidate& c) { return c.is_touchpad && c.uniq == pad.uniq; });

		if (touchpad == found.end())
			continue;

		Ds4Paths out;
		out.uniq           = pad.uniq;
		out.gamepad_event  = pad.path;
		out.touchpad_event = touchpad->path;

		const std::string hid_dir = HidDirOf(pad.path);
		if (hid_dir.empty())
			throw std::runtime_error("Cannot resolve HID device of " + pad.path);

		FindLeds(hid_dir, out);
		FindBattery(hid_dir, out);

		if (require_sysfs)
		{
			if (out.led_r.empty() || out.led_g.empty()
				|| out.led_b.empty() || out.led_global.empty())
				throw std::runtime_error("LED sysfs nodes not found under " + hid_dir);

			if (out.battery_capacity.empty())
				throw std::runtime_error("Battery sysfs nodes not found under " + hid_dir);
		}

		std::clog << "DS4 " << out.uniq
			<< ": gamepad=" << out.gamepad_event
			<< " touchpad=" << out.touchpad_event << std::endl;

		return out;
	}

	if (!wanted.empty())
		throw std::runtime_error("No DualShock 4 found with MAC " + mac_filter);

	throw std::runtime_error("DualShock 4 found, but its touchpad node is missing");
}
