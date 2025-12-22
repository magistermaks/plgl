
#include "context.hpp"

using namespace plgl;

float friction = 0.999f;
float gravity = 0.0f;
float surface = 0.999;

struct Point {
	Vec2 pos;
	Vec2 prv;
	float radius;

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

	void constrain() {

		const auto vec = velocity();
		const auto lower = radius;
		const auto upper_x = width - radius;
		const auto upper_y = height - radius;

		if (pos.x < lower) {
			pos.x = lower;
			prv.x = lower + vec.x * surface;
		}

		if (pos.y < lower) {
			pos.y = lower;
			prv.y = lower + vec.y * surface;
		}

		if (pos.x > upper_x) {
			pos.x = upper_x;
			prv.x = upper_x + vec.x * surface;
		}

		if (pos.y > upper_y) {
			pos.y = upper_y;
			prv.y = upper_y + vec.y * surface;
		}

	}
};

struct Line {
	Point* a;
	Point* b;

	void update() {
		const Vec2 delta = a->pos - b->pos;
		const float length = delta.length();
		const float scale = ((20 - length) / length) * 0.5f;

		a->pos += delta * scale;
		b->pos -= delta * scale;
	}
};

struct Group {

	RGBA color = HSLA(
		uniform_random(0, TAU),
		0.7f, 0.5f
	).rgba();

	std::vector<Point*> points;
	std::vector<Line> lines;

	Group(float x, float y, int links) {
		Point* first = add_point(nullptr, x, y);
		Point* last = first;

		for (int i = 0; i < links; i ++) {
			last = add_point(last, x + uniform_random(-5, 5), y + uniform_random(-5, 5));
		}

		lines.emplace_back(first, last);
	}

	~Group() {
		for (Point* p : points) {
			delete p;
		}

		points.clear();
	}

	Point* add_point(Point* prev, float x, float y) {
		auto* new_point = new Point({x, y}, {x, y}, 2);
		points.push_back(new_point);

		if (prev != nullptr) {
			lines.emplace_back(prev, new_point);
		}

		return new_point;
	}

	void update() {

		Vec2 avg = points[0]->pos;

		for (int i = 1; i < points.size(); i ++) {
			avg += points[i]->pos;
		}

		avg /= points.size();

		line(avg - Vec2 {4, 0}, avg + Vec2 {4, 0});
		line(avg - Vec2 {0, 4}, avg + Vec2 {0, 4});

		for (Point* p : points) {
			p->update();
			Vec2 vec = (p->pos - avg).norm() * 0.1;
			p->pos += vec;
		}

		for (Line l : lines) {
			l.update();
		}

	}

	void draw() {
		stroke(color);
		fill(color);

		update();

		for (Point* p : points) {
			circle(p->pos, p->radius);
		}

		for (Line l : lines) {
			line(l.a->pos, l.b->pos);
		}
	}
};

std::vector<Group*> groups;

int main() {

	open("Verlet Integration", 400, 300);
	background(255, 255, 255);

	// create short demo chain
	for (int i = 0; i < 10; i ++) {
		groups.emplace_back(new Group(
			uniform_random(0, width), // x
			uniform_random(0, height), // y
			(int) uniform_random(5, 30) // node count
		));
	}

	while (!should_close) {

		for (Group* g : groups) {
			g->draw();
		}

		swap();

	}

	for (Group* g : groups) {
		delete g;
	}

	close();

}
