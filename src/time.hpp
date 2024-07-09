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

	class Time {

		private:

			// https://linux.die.net/man/3/localtime
			const std::chrono::time_point<ClockType> point;
			const time_t time;
			const tm local;

			friend inline Duration duration(const Time& start, const Time& end);

		public:

			Time(std::chrono::time_point<ClockType> point)
			: point(point), time(ClockType::to_time_t(point)), local(*localtime(&time)) {}

			static Time now() {
				return {ClockType::now()};
			}

		public:

			int year() {
				return local.tm_year + 1900;
			}

			int month() {
				return local.tm_mon + 1;
			}

			int day() {
				return local.tm_mday;
			}

			int hour() {
				return local.tm_hour;
			}

			int minute() {
				return local.tm_min;
			}

			int second() {
				return local.tm_sec;
			}

		public:

			// day of the year
			int year_day() {
				return local.tm_yday + 1;
			}

			// day of the week
			int week_day() {
				return local.tm_wday + 1;
			}

			// milliseconds since Unix Epoch
			size_t mils_since_epoch() {
				return std::chrono::duration_cast<std::chrono::milliseconds>(point.time_since_epoch()).count();
			}

			// seconds since Unix Epoch
			size_t secs_since_epoch() {
				return std::chrono::duration_cast<std::chrono::seconds>(point.time_since_epoch()).count();
			}

	};

	class Duration {

		public:

			const int hours, minutes, seconds, milliseconds, microseconds, nanoseconds;

		public:

			Duration(int hors, int mins, int secs, int mils = 0, int mics = 0, int nans = 0)
			: hours(hors), minutes(mins), seconds(secs), milliseconds(mils), microseconds(mics), nanoseconds(nans) {}


	};

	inline Duration duration(const Time& start, const Time& end) {
		auto [hors, mins, secs, mils, mics, nans] = impl::split_duration<
			std::chrono::hours, std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds
		>(end.point - start.point);

		return Duration {hors, mins, secs, mils, mics, nans};
	}

	// Usage:
	//  * sleep(3s);                            - sleep for 3 seconds
	//  * sleep(300ms);                         - sleep for 300 milliseconds
	//  * sleep(std::chrono::milliseconds(10)); - sleep for 10 milliseconds
	template <class Rep, class Period>
	void sleep(const std::chrono::duration<Rep, Period>& duration){
		std::this_thread::sleep_for(duration);
	}

}
