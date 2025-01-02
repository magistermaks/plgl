
#include "math.hpp"

/**
 * For reference on the random vector picking see:
 * https://mathworld.wolfram.com/SpherePointPicking.html
 */

/*
 * Vec2
 */

plgl::Vec2 plgl::Vec2::random() {
	return from_angle(uniform_random(TAU));
}

plgl::Vec2 plgl::Vec2::random(Random& random) {
	return from_angle(random.uniform(0, TAU));
}

plgl::Vec2 plgl::Vec2::from_angle(float alpha) {
	return {(float) cos(alpha), (float) sin(alpha)};
}

float plgl::Vec2::quadrance() const {
	return x * x + y * y;
}

float plgl::Vec2::length() const {
	return sqrt(quadrance());
}

float plgl::Vec2::get(int index) const {
	return get_nth_element<float>(index, x, y);
}

plgl::Vec2 plgl::Vec2::norm() const {
	return *this / length();
}

plgl::Vec2 plgl::Vec2::perp() const {
	return {-y, x};
}

/*
 * Vec3
 */

plgl::Vec3 plgl::Vec3::random() {
	return from_angle(TAU * uniform_random(1), acos(2 * uniform_random(1) - 1));
}

plgl::Vec3 plgl::Vec3::random(Random& random) {
	return from_angle(TAU * random.uniform(0, 1), acos(2 * random.uniform(0, 1) - 1));
}

plgl::Vec3 plgl::Vec3::from_angle(float alpha, float beta) {
	return {(float) cos(alpha) * (float) cos(beta), (float) sin(beta), (float) sin(alpha) * (float) cos(beta)};
}

float plgl::Vec3::quadrance() const {
	return x * x + y * y + z * z;
}

float plgl::Vec3::length() const {
	return sqrt(quadrance());
}

float plgl::Vec3::get(int index) const {
	return get_nth_element<float>(index, x, y, z);
}

plgl::Vec3 plgl::Vec3::norm() const {
	return *this / length();
}

/*
 * functions
 */

float plgl::dist(const plgl::Vec2& a, const plgl::Vec2& b) {
	return (a - b).length();
}

float plgl::dist(const plgl::Vec3& a, const plgl::Vec3& b) {
	return (a - b).length();
}

float plgl::dist(float x1, float y1, float x2, float y2) {
	return plgl::dist(plgl::Vec2(x1, y1), plgl::Vec2(x2, y2));
}

float plgl::dist(float x1, float y1, float z1, float x2, float y2, float z2) {
	return plgl::dist(plgl::Vec3(x1, y1, z1), plgl::Vec3(x2, y2, z2));
}