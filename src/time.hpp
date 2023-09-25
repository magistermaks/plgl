//#pragma once
//
//#include "external.hpp"
//
//namespace plgl {
//
//	class Time {
//
//		private:
//
//			const std::chrono::time_point<std::chrono::system_clock> point;
//
//		public:
//
//			Time(std::chrono::time_point<std::chrono::system_clock> point)
//			: point(point) {}
//
//			static inline Time now() {
//				return {std::chrono::system_clock::now()};
//			}
//
//		public:
//
//			long year() {
//
//			}
//
//			long month() {
//
//			}
//
//	};
//
//unsigned long epoch() {
//		const auto now = std::chrono::system_clock::now();
//		const auto epoch = now.time_since_epoch();
//
//		// cast the duration into seconds
//		const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);
//		
//		// return the number of seconds
//		return seconds.count();
//	}
//
//
//}
//
