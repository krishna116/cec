/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#pragma once

#include "config.h"

#include <string>
#include <vector>
#include <iostream>

namespace cec
{
	struct EnumClass
	{
		struct Comment
		{
			std::string headComment;
			std::vector<std::string> keyComments;
		};
		std::string name;
		std::string fullName;
		std::string type;
		std::vector<std::string> keys;
		std::int64_t number = 0;
		Comment comment;
	};

	template <typename T>
	void print_helper_(T &&arg)
	{
		std::cout << arg;
	}

	template <typename First, typename... Rest>
	void print_helper_(First &&first, Rest &&...rest)
	{
		std::cout << first;
		print_helper_(std::forward<Rest>(rest)...);
	}

	// print_info
	template <typename First, typename... Rest>
	void print_info(First &&first, Rest &&...rest)
	{
		std::cout << "[" << config::AppName << "]";
		print_helper_(first, std::forward<Rest>(rest)...);
	}

	// print_error
	template <typename First, typename... Rest>
	void print_error(First &&first, Rest &&...rest)
	{
		std::cout << "[" << config::AppName << "]";
		std::cout << "[" << "error" << "]";
		print_helper_(first, std::forward<Rest>(rest)...);
	}

} // namespace cec
