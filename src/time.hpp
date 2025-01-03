#pragma once

#include "external.hpp"

namespace plgl {

	using namespace std::chrono_literals;
	using ClockType = std::chrono::system_clock;

	class Duration;
	class Time;

	namespace impl {

		template <typename T, typename R>
		using Lie = R;

		template <size_t... Is, typename T1, typename T2>
		inline T2& unpack_durations(T1&& s, T2& t, std::index_sequence<Is...>) {
			((
				std::get<Is>(t) = std::get<Is>(std::forward<T1>(s)).count()
			), ...);

			return t;
		}

		// based on https://stackoverflow.com/a/42139394
		template <class... Durations>
		inline std::tuple<Lie<Durations, int>...> split_duration(ClockType::duration diff) {
			std::tuple<Durations...> tuple;
			std::tuple<Lie<Durations, int>...> values;

			((
				std::get<Durations>(tuple) = std::chrono::duration_cast<Durations>(diff),
				diff -= std::chrono::duration_cast<ClockType::duration>(std::get<Durations>(tuple))
			), ...);

			return unpack_durations(tuple, values, std::make_index_sequence<sizeof...(Durations)>());
		}

	};

	/**
	 * Represents a point in time.
	 */
	class Time {

		private:

			// https://linux.die.net/man/3/localtime
			const std::chrono::time_point<ClockType> point;
			const time_t time;
			const tm local;

			friend inline Duration duration(const Time& start, const Time& end);

		public:

			Time(std::chrono::time_point<ClockType> point);

			/// Returns the current time
			static Time now();

		public:

			/// Year date component
			int year();

			/// Month date component
			int month();

			/// Day date component
			int day();

			/// Hour time component
			int hour();

			/// Minute time component
			int minute();

			/// Second time component
			int second();

		public:

			/// Day of the year
			int year_day();

			/// Day of the week
			int week_day();

			/// Milliseconds since Unix Epoch
			size_t mils_since_epoch();

			/// Seconds since Unix Epoch
			size_t secs_since_epoch();

	};

	/**
	 * Represents a difference between two points in time.
	 */
	class Duration {

		public:

			const int hours, minutes, seconds, milliseconds, microseconds, nanoseconds;

		public:

			Duration(int hors, int mins, int secs, int mils = 0, int mics = 0, int nans = 0);

	};

	Duration duration(const Time& start, const Time& end);

	// Usage:
	//  * sleep(3s);                            - sleep for 3 seconds
	//  * sleep(300ms);                         - sleep for 300 milliseconds
	//  * sleep(std::chrono::milliseconds(10)); - sleep for 10 milliseconds
	template <class Rep, class Period>
	void sleep(const std::chrono::duration<Rep, Period>& duration){
		std::this_thread::sleep_for(duration);
	}

}
