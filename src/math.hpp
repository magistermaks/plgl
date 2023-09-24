#pragma once

#include "external.hpp"
#include "util.hpp"

namespace plgl {

	constexpr float PI = 3.14159265358979323846f;
	constexpr float TAU = 2 * PI;
	constexpr float HALF_PI = 0.5f * PI;
	constexpr float EULER = 2.71828182845904523536f;

	template<typename T>
	inline T clamp(T value, T low, T high) {
		if (value > high) return high;
		if (value < low) return low;
		return value;
	} 

	class Vec2 {
		
		public:

			const float x, y;

			Vec2(float x, float y)
			: x(x), y(y) {}

			// https://mathworld.wolfram.com/SpherePointPicking.html
			static inline Vec2 random() {
				return from_angle(uniform_random(TAU));
			}

			static inline Vec2 from_angle(float alpha) {
				return {cos(alpha), sin(alpha)};
			}

		public:

			Vec2 operator / (float scalar) const {
				return {x / scalar, y / scalar};
			}
			
			Vec2 operator * (float scalar) const {
				return {x * scalar, y * scalar};
			}

			Vec2 operator + (float scalar) const {
				return {x + scalar, y + scalar};
			}

			Vec2 operator - (float scalar) const {
				return {x - scalar, y - scalar};
			}

			Vec2 operator + (const Vec2& vector) const {
				return {x + vector.x, y + vector.y};
			}

			Vec2 operator - (const Vec2& vector) const {
				return {x - vector.x, y - vector.y};
			}
	
		public:	

			float length() const {
				return sqrt(x * x + y * y);
			}

			float get(int index) const {
				return get_nth_element<float>(index, x, y);
			}

			Vec2 normalized() const {
				return *this / length();
			}
			

	};

	class Vec3 {
		
		public:

			const float x, y, z;

			Vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}

			// https://mathworld.wolfram.com/SpherePointPicking.html
			static inline Vec3 random() {
				return from_angle(TAU * uniform_random(1), acos(2 * uniform_random(1) - 1));
			}

			static inline Vec3 from_angle(float alpha, float beta) {
				return {cos(alpha) * cos(beta), sin(beta), sin(alpha) * cos(beta)};
			}

		public:

			Vec3 operator / (float scalar) const {
				return {x / scalar, y / scalar, z / scalar};
			}

			Vec3 operator * (float scalar) const {
				return {x * scalar, y * scalar, z * scalar};
			}

			Vec3 operator + (float scalar) const {
				return {x + scalar, y + scalar, z + scalar};
			}

			Vec3 operator - (float scalar) const {
				return {x - scalar, y - scalar, z - scalar};
			}

			Vec3 operator + (const Vec3& vector) const {
				return {x + vector.x, y + vector.y, z + vector.z};
			}

			Vec3 operator - (const Vec3& vector) const {
				return {x - vector.x, y - vector.y, z - vector.z};
			}

		public:

			float length() const {
				return sqrt(x * x + y * y + z * z);
			}

			float get(int index) const {
				return get_nth_element<float>(index, x, y, z);
			}

			Vec3 normalized() const {
				return *this / length();
			}

	};

	inline float dot(const Vec2& a, const Vec2& b) {
		return a.x * b.x + a.y * b.y;
	}

	inline float dot(const Vec3& a, const Vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline float cross(const Vec2& a, const Vec2& b) {
		return a.x * b.y - a.y * b.x;
	}

	inline Vec3 cross(const Vec3& a, const Vec3& b) {
		return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
	}

	inline float dist(const Vec2& a, const Vec2& b) {
		return (a - b).length();
	} 

	inline float dist(const Vec3& a, const Vec3& b) {
		return (a - b).length();
	} 

	inline float dist(float x1, float y1, float x2, float y2) {
		return dist(Vec2(x1, y1), Vec2(x2, y2));
	} 

	inline float dist(float x1, float y1, float z1, float x2, float y2, float z2) {
		return dist(Vec3(x1, y1, z1), Vec3(x2, y2, z2));
	}

	template <typename... Args>
	inline auto min(Args... args) {
		return std::min({args...});
	}

	template <typename... Args>
	inline auto max(Args... args) {
		return std::max({args...});
	}

	// remaps given value from range [cs, ce] to range [ts, te]
	inline float remap(float value, float cs, float ce, float ts, float te) {
		return (value - cs) / (ce - cs) * (te - ts) + ts;
	}

}

