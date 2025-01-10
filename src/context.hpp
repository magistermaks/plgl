#pragma once

#include "render/renderer.hpp"
#include "sound/system.hpp"
#include "window.hpp"
#include "color.hpp"
#include "globals.hpp"
#include "time.hpp"

namespace plgl {

	/*
	 * Renderer
	 */

	inline void stroke(Disabled disabled) {
		renderer->stroke(disabled);
	}

	inline void stroke(float r, float g, float b, float a = 255) {
		renderer->stroke(r, g, b, a);
	}

	inline void stroke(const Color& color) {
		renderer->stroke(color);
	}

	inline void weight(float w) {
		renderer->weight(w);
	}

	inline void quality(Quality q) {
		renderer->quality(q);
	}

	inline void fill(Disabled disabled) {
		renderer->fill(disabled);
	}

	inline void fill(float r, float g, float b, float a = 255) {
		renderer->fill(r, g, b, a);
	}

	inline void fill(const Color& color) {
		renderer->fill(color);
	}

	inline void tint(Disabled disabled) {
		renderer->tint(disabled);
	}

	inline void tint(float r, float g, float b, float a = 255) {
		renderer->tint(r, g, b, a);
	}

	inline void tint(const Color& color) {
		renderer->tint(color);
	}

	inline void clip(Disabled disabled) {
		renderer->clip(disabled);
	}

	inline void clip(float x1, float y1, float x2, float y2) {
		renderer->clip(x1, y1, x2, y2);
	}

	inline void texture(Sprite& sprite) {
		renderer->texture(sprite);
	}

	inline void texture(Texture& t, float bx, float by, float ex, float ey) {
		renderer->texture(t, bx, by, ex, ey);
	}

	inline void texture(Texture& t) {
		renderer->texture(t);
	}

	inline void font(Font& f) {
		renderer->font(f);
	}

	inline void size(float s) {
		renderer->size(s);
	}

	inline void arc(float x, float y, float hrad, float vrad, float start, float angle, ArcMode mode = OPEN_PIE) {
		renderer->arc(x, y, hrad, vrad, angle, start, mode);
	}

	inline float bezier_point(float a, float b, float c, float d, float t) {
		return renderer->bezier_tangent(a, b, c, d, t);
	}

	inline float bezier_tangent(float a, float b, float c, float d, float t) {
		return renderer->bezier_tangent(a, b, c, d, t);
	}

	inline void bezier(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy) {
		renderer->bezier(ax, ay, bx, by, cx, cy, dx, dy);
	}

	inline void bezier(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
		renderer->bezier(a, b, c, d);
	}

	inline void circle(float x, float y, float radius) {
		renderer->circle(x, y, radius);
	}

	inline void ellipse(float x, float y, float hr, float vr) {
		renderer->ellipse(x, y, hr, vr);
	}

	inline void line(float x1, float y1, float x2, float y2) {
		renderer->line(x1, y1, x2, y2);
	}

	inline void point(float x, float y) {
		renderer->point(x, y);
	}

	inline void trig(float x1, float y1, float x2, float y2, float x3, float y3) {
		renderer->trig(x1, y1, x2, y2, x3, y3);
	}

	inline void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
		renderer->quad(x1, y1, x2, y2, x3, y3, x4, y4);
	}

	inline void rect(float x, float y, float w, float h, float r) {
		renderer->rect(x, y, w, h, r);
	}

	inline void rect(float x, float y, float w, float h, float r1, float r2, float r3, float r4) {
		renderer->rect(x, y, w, h, r1, r2, r3, r4);
	}

	inline void square(float x, float y, float extent) {
		renderer->square(x, y, extent);
	}

	inline void image(float x, float y, float w, float h) {
		renderer->image(x, y, w, h);
	}

	inline void image(float x, float y) {
		renderer->image(x, y);
	}

	inline void text(float x, float y, const std::string& str) {
		renderer->text(x, y, str);
	}

	template<class... Args>
	void textf(float x, float y, const std::string& str, Args&&... args) {
		renderer->textf(x, y, str, args...);
	}

	inline void quad(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4) {
		renderer->quad(p1, p2, p3, p4);
	}

	inline void trig(Vec2 p1, Vec2 p2, Vec2 p3) {
		renderer->trig(p1, p2, p3);
	}

	inline void line(Vec2 p1, Vec2 p2) {
		renderer->line(p1, p2);
	}

	inline void point(Vec2 p1) {
		renderer->point(p1);
	}

	inline void circle(Vec2 p1, float radius) {
		renderer->circle(p1, radius);
	}

	inline void ellipse(Vec2 p1, float hrad, float vrad) {
		renderer->ellipse(p1, hrad, vrad);
	}

	/*
	 * Sound
	 */

	inline void stop_sounds() {
		SoundSystem::get()->stop_all();
	}

	inline void pause_sounds() {
		SoundSystem::get()->pause_all();
	}

	inline void resume_sounds() {
		SoundSystem::get()->resume_all();
	}

	inline Source tone(Waveform waveform, float volume, float frequency, int milliseconds = 100) {
		SoundSystem::get()->tone(waveform, volume, frequency, milliseconds);
	}

};
