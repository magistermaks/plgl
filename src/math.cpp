
#include "math.hpp"

/** \file math.cpp
 * For reference on the random vector picking see:
 * https://mathworld.wolfram.com/SpherePointPicking.html
 */

namespace plgl {

	/*
	 * Vec2
	 */

	Vec2 Vec2::random() {
		return from_angle(uniform_random(TAU));
	}

	Vec2 Vec2::random(Random& random) {
		return from_angle(random.uniform(0, TAU));
	}

	Vec2 Vec2::from_angle(float alpha) {
		return {(float) cos(alpha), (float) sin(alpha)};
	}

	float Vec2::quadrance() const {
		return x * x + y * y;
	}

	float Vec2::length() const {
		return sqrt(quadrance());
	}

	float Vec2::get(int index) const {
		return get_nth_element<float>(index, x, y);
	}

	Vec2 Vec2::norm() const {
		return *this / length();
	}

	Vec2 Vec2::perp() const {
		return {-y, x};
	}

	/*
	 * Vec3
	 */

	Vec3 Vec3::random() {
		return from_angle(TAU * uniform_random(1), acos(2 * uniform_random(1) - 1));
	}

	Vec3 Vec3::random(Random& random) {
		return from_angle(TAU * random.uniform(0, 1), acos(2 * random.uniform(0, 1) - 1));
	}

	Vec3 Vec3::from_angle(float alpha, float beta) {
		return {(float) cos(alpha) * (float) cos(beta), (float) sin(beta), (float) sin(alpha) * (float) cos(beta)};
	}

	float Vec3::quadrance() const {
		return x * x + y * y + z * z;
	}

	float Vec3::length() const {
		return sqrt(quadrance());
	}

	float Vec3::get(int index) const {
		return get_nth_element<float>(index, x, y, z);
	}

	Vec3 Vec3::norm() const {
		return *this / length();
	}

	/*
	 * functions
	 */

	float dist(const Vec2& a, const Vec2& b) {
		return (a - b).length();
	}

	float dist(const Vec3& a, const Vec3& b) {
		return (a - b).length();
	}

	float dist(float x1, float y1, float x2, float y2) {
		return dist(Vec2(x1, y1), Vec2(x2, y2));
	}

	float dist(float x1, float y1, float z1, float x2, float y2, float z2) {
		return dist(Vec3(x1, y1, z1), Vec3(x2, y2, z2));
	}

}