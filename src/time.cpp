
#include "time.hpp"
#include "util.hpp"

namespace plgl {

	/*
	 * Time
	 */

	Time::Time(std::chrono::time_point<ClockType> point)
	: point(point), time(ClockType::to_time_t(point)), local(*localtime(&time)) {}

	Time Time::now() {
		return {ClockType::now()};
	}

	int Time::year() {
		return local.tm_year + 1900;
	}

	int Time::month() {
		return local.tm_mon + 1;
	}

	int Time::day() {
		return local.tm_mday;
	}

	int Time::hour() {
		return local.tm_hour;
	}

	int Time::minute() {
		return local.tm_min;
	}

	int Time::second() {
		return local.tm_sec;
	}

	int Time::year_day() {
		return local.tm_yday + 1;
	}

	int Time::week_day() {
		return local.tm_wday + 1;
	}

	size_t Time::mils_since_epoch() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(point.time_since_epoch()).count();
	}

	size_t Time::secs_since_epoch() {
		return std::chrono::duration_cast<std::chrono::seconds>(point.time_since_epoch()).count();
	}

	/*
	 * Duration
	 */

	Duration::Duration(int hors, int mins, int secs, int mils, int mics, int nans)
	: hours(hors), minutes(mins), seconds(secs), milliseconds(mils), microseconds(mics), nanoseconds(nans) {}

	/*
	 * functions
	 */

	Duration duration(const Time& start, const Time& end) {
		auto [hors, mins, secs, mils, mics, nans] = impl::split_duration<
		std::chrono::hours, std::chrono::minutes, std::chrono::seconds, std::chrono::milliseconds, std::chrono::microseconds, std::chrono::nanoseconds
		>(end.point - start.point);

		return Duration {hors, mins, secs, mils, mics, nans};
	}

}