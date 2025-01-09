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

			// we don't really need to escape '}' by c++ tells us to do
			// that anyway so i kinda do but allow also using '}' as is
			if (last == '}') {
				if (chr != '}') {
					formatted += chr;
				}
			}

			if (last == '{') {
				if (chr == '{') formatted += '{';

				try {
					if (chr == '}') formatted += arguments.at(index ++);
				} catch (...) {
					return formatted;
				}

				last = 0;
				continue;
			}

			if (chr != '{') {
				formatted += chr;
			}

			last = chr;
		}

		return formatted;

	}

	template<typename... Args>
	[[noreturn]] void fault(const std::string& format_string, Args&&... args) {
		throw std::runtime_error {format(format_string, args...)};
	}

	template <typename C, typename E>
	inline bool contains(const C& collection, const E& element) {
		return std::find(std::begin(collection), std::end(collection), element) != std::end(collection);
	}

}
