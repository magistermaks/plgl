
#include "context.hpp"

struct Group;
struct Point;
using namespace plgl;

float friction = 0.999f;
float gravity = 0.0f;
float surface = 0.999;
bool internal_collisions = false;

struct Line {
	Point* a;
	Point* b;
	Group* group;

	void update();
};

struct Point {
	Vec2 pos;
	Vec2 prv;
	float radius;
	Group* group;

	Vec2 velocity() const {
		return (pos - prv) * friction;
	}

	void update() {
		Vec2 vec = velocity();

		prv = pos;
		pos += vec;

		// apply gravity
		pos.y += gravity;

		constrain();
	}

	void try_collide(Point* other) {
		float ds = (other->pos - pos).quadrance();
		float md = other->radius + radius;

		if (other == this) {
			return;
		}

		if (md * md > ds) {

			// vector from this particle to other
			Vec2 vec = other->pos - pos;
			float length = sqrtf(ds);

			// vector used to adjust particle position
			Vec2 back = (vec * (float) ((md - length) / 2.0f) / length);

			// adjust positions
			pos -= back;
			other->pos += back;
		}
	}

	void constrain() {

		const auto vec = velocity();
		const auto lower = radius;
		const auto upper_x = width - radius;
		const auto upper_y = height - radius;

		if (pos.x < lower) {
			pos.x = lower;
			prv.x = lower; // + vec.x * surface;
		}

		if (pos.y < lower) {
			pos.y = lower;
			prv.y = lower; // + vec.y * surface;
		}

		if (pos.x > upper_x) {
			pos.x = upper_x;
			prv.x = upper_x; // + vec.x * surface;
		}

		if (pos.y > upper_y) {
			pos.y = upper_y;
			prv.y = upper_y; // + vec.y * surface;
		}

	}
};

struct Group {

	RGBA color = HSLA(
		uniform_random(0, TAU), // pick random hue
		0.7f, 0.5f
	).rgba();

	std::vector<Point*> points;

	Vec2 center() const {
		Vec2 avg = points[0]->pos;

		for (int i = 1; i < points.size(); i ++) {
			avg += points[i]->pos;
		}

		return avg / points.size();
	}

	void update() {
		Vec2 avg = center();

		stroke(color);
		line(avg - Vec2 {4, 0}, avg + Vec2 {4, 0});
		line(avg - Vec2 {0, 4}, avg + Vec2 {0, 4});

		for (Point* p : points) {

			// push point away from ring center
			Vec2 vec = (p->pos - avg).norm() * 0.1;
			p->pos += vec;
		}
	}

};

void Line::update() {
	const Vec2 delta = a->pos - b->pos;
	const float length = delta.length();
	const float scale = ((10 - length) / length) * 0.5f;

	a->pos += delta * scale;
	b->pos -= delta * scale;
}

std::vector<Point*> points;
std::vector<Line> lines;
std::vector<Group*> groups;

Point* add_point(Point* prev, float x, float y, Group* group) {
	auto* new_point = new Point({x, y}, {x, y}, 4, group);
	group->points.emplace_back(new_point);
	points.push_back(new_point);

	if (prev != nullptr) {
		lines.emplace_back(prev, new_point, group);
	}

	return new_point;
}

void add_ring(float x, float y, int links) {
	auto* group = new Group();

	Point* first = add_point(nullptr, x, y, group);
	Point* last = first;

	for (int i = 0; i < links; i ++) {
		last = add_point(last, x + uniform_random(-5, 5), y + uniform_random(-5, 5), group);
	}

	// close the ring
	lines.emplace_back(first, last, group);
	groups.emplace_back(group);
}

void update() {
	for (Point* p : points) {
		p->update();
	}

	for (Group* g : groups) {
		g->update();
	}

	for (int i = 0; i < points.size(); i ++) {
		for (int j = i; j < points.size(); j ++) {
			Point* first = points[i];
			Point* second = points[j];

			if ((first->group != second->group) || internal_collisions) {
				first->try_collide(second);
			}
		}
	}

	for (Line l : lines) {
		l.update();
	}

	// draw

	for (Point* p : points) {
		stroke(p->group->color);
		fill(p->group->color);
		circle(p->pos, p->radius);
	}

	for (Line l : lines) {
		stroke(l.a->group->color);
		line(l.a->pos, l.b->pos);
	}
}

int main() {

	open("Verlet Integration", 400, 300);
	background(255, 255, 255);

	add_ring(
		uniform_random(0, width), // x
		uniform_random(0, height), // y
		(int) uniform_random(5, 30) // node count
	);

	listen(MOUSE_PRESSED, [] {
		add_ring(mouse_x, mouse_y, (int) uniform_random(5, 30));
	});

	listen(KEY_PRESSED, [] {
		if (key_code == 'c') internal_collisions = !internal_collisions;
	});

	Font sans {"default"};

	size(20);
	tint(0, 0, 0);
	font(sans);

	while (!should_close) {
		update();
		text(5, 15, "Internal Collisions: " + std::string(internal_collisions ? "Enabled" : "Disabled"));
		swap();
	}

	// free memory
	for (Group* g : groups) delete g;
	for (Point* p : points) delete p;

	close();

}
