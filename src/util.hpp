#pragma once

#include "random.hpp"

namespace plgl {

	namespace trait {

		template <typename T>
		concept ConvertibleToStdString = requires(T value) { std::to_string(value); };

		template <typename T>
		concept CastableToStdString = std::convertible_to<T, std::string>;

		template <typename T>
		concept AppendableToStdString = requires(T value, std::stringstream ss) { ss << value; } && !ConvertibleToStdString<T> && !CastableToStdString<T>;
	}

	template <typename T>
	inline std::string str(const T& value) {
		return typeid(value).name();
	}

	template <trait::ConvertibleToStdString T>
	inline std::string str(const T& value) {
		return std::to_string(value);
	}

	template <trait::CastableToStdString T>
	inline std::string str(const T& value) {
		return value;
	}

	template <trait::AppendableToStdString T>
	inline std::string str(const T& value) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	template <typename T, class... Args>
	T get_nth_element(int i, Args... args) {
		return static_cast<T>(std::array {args...}.at(i));
	}

	template<class... Args>
	inline std::string format(const std::string& format_str, Args&&... args) {
				
		std::vector<std::string> arguments;
		(arguments.emplace_back(str(args)), ...);

		std::string formatted;
		char last = 0;
		int index = 0;

		for (char chr : format_str) {
			if (last == '%') {
				if (chr == '%') formatted += '%';

				try {
					if (chr == 's') formatted += arguments.at(index ++);
				} catch (...) {
					return formatted;
				}

				last = 0;
				continue;
			}

			if (chr != '%') {
				formatted += chr;
			}

			last = chr;
		}

		return formatted;

	}

}

