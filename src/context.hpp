#pragma once

#include "render/renderer.hpp"
#include "window.hpp"
#include "color.hpp"
#include "globals.hpp"
#include "time.hpp"

namespace plgl {

	void stroke(Disabled disabled) {
		renderer->stroke(disabled);
	}

	void stroke(float r, float g, float b, float a = 255) {
		renderer->stroke(r, g, b, a);
	}

	void weight(float w) {
		renderer->weight(w);
	}

	void quality(Quality q) {
		renderer->quality(q);
	}

	void fill(Disabled disabled) {
		renderer->fill(disabled);
	}

	void fill(float r, float g, float b, float a = 255) {
		renderer->fill(r, g, b, a);
	}

	void tint(Disabled disabled) {
		renderer->tint(disabled);
	}

	void tint(float r, float g, float b, float a = 255) {
		renderer->tint(r, g, b, a);
	}

	void clip(Disabled disabled) {
		renderer->clip(disabled);
	}

	void clip(float x1, float y1, float x2, float y2) {
		renderer->clip(x1, y1, x2, y2);
	}

	void texture(Texture& t, float bx, float by, float ex, float ey) {
		renderer->texture(t, bx, by, ex, ey);
	}

	void texture(Texture& t) {
		renderer->texture(t);
	}

	void font(Font& f) {
		renderer->font(f);
	}

	void size(float s) {
		renderer->size(s);
	}

	void arc(float x, float y, float hrad, float vrad, float start, float angle, ArcMode mode = OPEN_PIE) {
		renderer->arc(x, y, hrad, vrad, angle, start, mode);
	}

	void circle(float x, float y, float radius) {
		renderer->circle(x, y, radius);
	}

	void ellipse(float x, float y, float hr, float vr) {
		renderer->ellipse(x, y, hr, vr);
	}

	void line(float x1, float y1, float x2, float y2) {
		renderer->line(x1, y1, x2, y2);
	}

	void point(float x, float y) {
		renderer->point(x, y);
	}

	void trig(float x1, float y1, float x2, float y2, float x3, float y3) {
		renderer->trig(x1, y1, x2, y2, x3, y3);
	}

	void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
		renderer->quad(x1, y1, x2, y2, x3, y3, x4, y4);
	}

	void rect(float x, float y, float w, float h, float r) {
		renderer->rect(x, y, w, h, r);
	}
		
	void rect(float x, float y, float w, float h, float r1, float r2, float r3, float r4) {
		renderer->rect(x, y, w, h, r1, r2, r3, r4);
	}

	void square(float x, float y, float extent) {
		renderer->square(x, y, extent);
	}

	void image(float x, float y, float w, float h) {
		renderer->image(x, y, w, h);
	}

	void image(float x, float y) {
		renderer->image(x, y);
	}

	void text(float x, float y, const std::string& str) {
		renderer->text(x, y, str);
	}

	template<class... Args>
	void textf(float x, float y, const std::string& str, Args&&... args) {
		renderer->textf(x, y, str, args...);
	}

};
