#pragma once

#include "external.hpp"
#include "util.hpp"

namespace plgl {

	constexpr float PI = 3.14159265358979323846f;
	constexpr float TAU = 2 * PI;
	constexpr float HALF_PI = 0.5f * PI;
	constexpr float EULER = 2.71828182845904523536f;
	constexpr float DEGREE = PI / 180.0f;

	class Vec2 {

		public:

			mutable float x, y;

			constexpr Vec2()
			: Vec2(0) {}

			constexpr Vec2(float v)
			: Vec2(v, v) {}

			constexpr Vec2(float x, float y)
			: x(x), y(y) {}

			static Vec2 random();
			static Vec2 random(Random& random);
			static Vec2 from_angle(float alpha);

		public:

			float quadrance() const;
			float length() const;
			float get(int index) const;

			/// returns a normalized vector
			Vec2 norm() const;

			/// returns a perpendicular vector
			Vec2 perp() const;

	};

	class Vec3 {

		public:

			mutable float x, y, z;

			constexpr Vec3()
			: Vec3(0) {}

			constexpr Vec3(float v)
			: Vec3(v, v, v) {}

			constexpr Vec3(float x, float y, float z)
			: x(x), y(y), z(z) {}

			static Vec3 random();
			static Vec3 random(Random& random);
			static Vec3 from_angle(float alpha, float beta);

		public:

			float quadrance() const;
			float length() const;
			float get(int index) const;

			/// returns a normalized vector
			Vec3 norm() const;

	};


	float dist(const Vec2& a, const Vec2& b);
	float dist(const Vec3& a, const Vec3& b);
	float dist(float x1, float y1, float x2, float y2);
	float dist(float x1, float y1, float z1, float x2, float y2, float z2);

	constexpr float dot(const Vec2& a, const Vec2& b) {
		return a.x * b.x + a.y * b.y;
	}

	constexpr float dot(const Vec3& a, const Vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	constexpr float cross(const Vec2& a, const Vec2& b) {
		return a.x * b.y - a.y * b.x;
	}

	constexpr plgl::Vec3 cross(const plgl::Vec3& a, const plgl::Vec3& b) {
		return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
	}

	constexpr float clamp(float value, float low, float high) {
		if (value > high) return high;
		if (value < low) return low;
		return value;
	}

	/// remaps given value from range [cs, ce] to range [ts, te]
	constexpr inline float remap(float value, float cs, float ce, float ts, float te) {
		return (value - cs) / (ce - cs) * (te - ts) + ts;
	}

	constexpr inline float deg(float rads) {
		return rads / plgl::DEGREE;
	}

	constexpr inline float rad(float degs) {
		return degs * plgl::DEGREE;
	}

	constexpr float cot(float v) {
		return tan(plgl::HALF_PI - v);
	}

	constexpr float acot(float v) {
		return acot(1.0f / v);
	}

	template <typename... Args>
	inline auto min(Args... args) {
		return std::min({args...});
	}

	template <typename... Args>
	inline auto max(Args... args) {
		return std::max({args...});
	}

}

/*
 * Vec2 Operators
 */

inline plgl::Vec2 operator / (const plgl::Vec2& lhs, float scalar) {
	return {lhs.x / scalar, lhs.y / scalar};
}

inline plgl::Vec2 operator * (const plgl::Vec2& lhs, float scalar) {
	return {lhs.x * scalar, lhs.y * scalar};
}

inline plgl::Vec2 operator + (const plgl::Vec2& lhs, float scalar) {
	return {lhs.x + scalar, lhs.y + scalar};
}

inline plgl::Vec2 operator - (const plgl::Vec2& lhs, float scalar) {
	return {lhs.x - scalar, lhs.y - scalar};
}

inline plgl::Vec2 operator + (const plgl::Vec2& lhs, const plgl::Vec2& rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline plgl::Vec2 operator - (const plgl::Vec2& lhs, const plgl::Vec2& rhs) {
	return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline plgl::Vec2 operator / (float scalar, const plgl::Vec2& rhs) {
	return {scalar / rhs.x, scalar / rhs.y};
}

inline plgl::Vec2 operator * (float scalar, const plgl::Vec2& rhs) {
	return {scalar * rhs.x, scalar * rhs.y};
}

inline plgl::Vec2 operator + (float scalar, const plgl::Vec2& rhs) {
	return {scalar + rhs.x, scalar + rhs.y};
}

inline plgl::Vec2 operator - (float scalar, const plgl::Vec2& rhs) {
	return {scalar - rhs.x, scalar - rhs.y};
}

/*
 * Vec3 Operators
 */

inline plgl::Vec3 operator / (const plgl::Vec3& lhs, float scalar) {
	return {lhs.x / scalar, lhs.y / scalar, lhs.z / scalar};
}

inline plgl::Vec3 operator * (const plgl::Vec3& lhs, float scalar) {
	return {lhs.x * scalar, lhs.y * scalar, lhs.z * scalar};
}

inline plgl::Vec3 operator + (const plgl::Vec3& lhs, float scalar) {
	return {lhs.x + scalar, lhs.y + scalar, lhs.z + scalar};
}

inline plgl::Vec3 operator - (const plgl::Vec3& lhs, float scalar) {
	return {lhs.x - scalar, lhs.y - scalar, lhs.z - scalar};
}

inline plgl::Vec3 operator + (const plgl::Vec3& lhs, const plgl::Vec3& rhs) {
	return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

inline plgl::Vec3 operator - (const plgl::Vec3& lhs, const plgl::Vec3& rhs) {
	return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

inline plgl::Vec3 operator / (float scalar, const plgl::Vec3& rhs) {
	return {scalar / rhs.x, scalar / rhs.y, scalar / rhs.z};
}

inline plgl::Vec3 operator * (float scalar, const plgl::Vec3& rhs) {
	return {scalar * rhs.x, scalar * rhs.y, scalar * rhs.z};
}

inline plgl::Vec3 operator + (float scalar, const plgl::Vec3& rhs) {
	return {scalar + rhs.x, scalar + rhs.y, scalar + rhs.z};
}

inline plgl::Vec3 operator - (float scalar, const plgl::Vec3& rhs) {
	return {scalar - rhs.x, scalar - rhs.y, scalar - rhs.z};
}