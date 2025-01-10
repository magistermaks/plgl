
#include "renderer.hpp"

namespace plgl {

	/*
	 * Renderer
	 */

	void Renderer::slanted_line(Vec2 p1, Vec2 d1, Vec2 p2, Vec2 d2) {
		use(color_pipeline);

		if (!stroke_flag) {
			return;
		}

		float half = stroke_width * 0.5f;

		// Calculate the vector along the end side of a segment line
		Vec2 s1 = d1.perp() * half;
		Vec2 s2 = d2.perp() * half;

		// Vertices near the P1 point
		Vec2 a1 = p1 + s1;
		Vec2 a2 = p1 - s1;

		// Vertices near the P2 point
		Vec2 b1 = p2 + s2;
		Vec2 b2 = p2 - s2;

		svert(a1.x, a1.y);
		svert(a2.x, a2.y);
		svert(b1.x, b1.y);

		svert(b1.x, b1.y);
		svert(a2.x, a2.y);
		svert(b2.x, b2.y);
	}

	Renderer::Renderer() {
		quality(MEDIUM);
		fill(255, 255, 255);
		tint(255, 255, 255);
		stroke(0, 0, 0);
		weight(1);
		size(20);
	}

	void Renderer::stroke(Disabled disabled) {
		this->stroke_flag = false;
	}

	void Renderer::stroke(float r, float g, float b, float a) {
		this->stroke_flag = true;
		this->sr = r;
		this->sg = g;
		this->sb = b;
		this->sa = a;
	}

	void Renderer::stroke(const Color& color) {
		RGBA rgba = color.as_rgba();
		stroke(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());
	}

	void Renderer::weight(float w) {
		this->stroke_width = w;
	}

	/// configures the max error allowed
	void Renderer::quality(Quality q) {
		this->draw_quality = ((float) q) * 0.1f;
	}

	void Renderer::fill(Disabled disabled) {
		this->fill_flag = false;
	}

	void Renderer::fill(float r, float g, float b, float a) {
		this->fill_flag = true;
		this->fr = r;
		this->fg = g;
		this->fb = b;
		this->fa = a;
	}

	void Renderer::fill(const Color& color) {
		RGBA rgba = color.as_rgba();
		fill(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());
	}

	void Renderer::tint(Disabled disabled) {
		tint(255, 255, 255);
	}

	void Renderer::tint(float r, float g, float b, float a) {
		this->tr = r;
		this->tg = g;
		this->tb = b;
		this->ta = a;
	}

	void Renderer::tint(const Color& color) {
		RGBA rgba = color.as_rgba();
		tint(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());
	}

	void Renderer::texture(Sprite& sprite) {
		texture(*sprite.texture, sprite.x, sprite.y, sprite.x + sprite.w, sprite.y + sprite.h);
	}

	void Renderer::texture(Texture& t, float bx, float by, float ex, float ey) {
		useTexture(t);

		float w = t.width();
		float h = t.height();

		this->bx = std::min(bx, w) / w;
		this->by = std::min(by, h) / h;
		this->ex = std::min(ex, w) / w;
		this->ey = std::min(ey, h) / h;

		this->tw = std::abs(bx - ex);
		this->th = std::abs(by - ey);
	}

	void Renderer::texture(Texture& t) {
		texture(t, 0, 0, t.width(), t.height());

		this->tw = t.width();
		this->th = t.height();
	}

	void Renderer::font(Font& f) {
		useFont(f);
	}

	void Renderer::size(float s) {
		this->text_size = s;
	}

	void Renderer::arc(float x, float y, float hrad, float vrad, float start, float angle, ArcMode mode) {

		float extension = getStrokeWidth();
		float herad = hrad + extension;
		float verad = vrad + extension;
		float extent = std::max(herad, verad);

		// https://stackoverflow.com/a/11774493
		float correctness = 1 - draw_quality / extent;
		int sides = std::max(3, (int) ceil(abs(angle) / acos(2 * correctness * correctness - 1)));
		float step = angle / sides;

		for (int i = 0; i < sides; i ++) {
			float ax = x + hrad * cos(start + step * i);
			float ay = y + vrad * sin(start + step * i);

			float bx = x + hrad * cos(start + step * (i + 1));
			float by = y + vrad * sin(start + step * (i + 1));

			if (fill_flag) {
				fvert(x, y);
				fvert(ax, ay);
				fvert(bx, by);
			}

			if (stroke_flag) {
				float cx = x + herad * cos(start + step * i);
				float cy = y + verad * sin(start + step * i);

				float dx = x + herad * cos(start + step * (i + 1));
				float dy = y + verad * sin(start + step * (i + 1));

				svert(ax, ay);
				svert(cx, cy);
				svert(dx, dy);

				svert(ax, ay);
				svert(dx, dy);
				svert(bx, by);
			}
		}

		if (angle > PI && mode == OPEN_CHORD && fill_flag) {
			float ax = x + hrad * cos(start);
			float ay = y + vrad * sin(start);

			float bx = x + hrad * cos(start + angle);
			float by = y + vrad * sin(start + angle);

			fvert(x, y);
			fvert(ax, ay);
			fvert(bx, by);
		}

	}

	float Renderer::bezier_point(float a, float b, float c, float d, float t) {
		const float it = 1 - t;

		const float it2 = it * it;
		const float it3 = it * it2;

		const float t2 = t * t;
		const float t3 = t * t2;

		return a * it3 + 3 * it2 * t * b + 3 * it * t2 * c + t3 * d;
	}

	float Renderer::bezier_tangent(float a, float b, float c, float d, float t) {
		const float it = 1 - t;

		const float it2 = it * it;
		const float t2 = t * t;

		return 3 * it2 * (b - a) + 6 * it * t * (c - b) + 3 * t2 * (d - c);
	}

	void Renderer::bezier(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy) {
		float t = 0;

		bool draw = false;
		Vec2 last_point;
		Vec2 last_tangent;

		// the curve will be no longer than this upper bound
		const float bound = dist(ax, ay, bx, by) + dist(bx, by, cx, cy) + dist(cx, cy, dx, dy);

		// Quality control specifies the spacing between T values that are sampled
		float spacing = 10.0f + 60.0f * draw_quality;
		float parts = bound / spacing;
		float segments = parts + (12 / parts + 3);
		float step = 1.0f / segments;

		// The last point is calculated outside the loop
		const float ex = bezier_tangent(ax, bx, cx, dx, 1.0f);
		const float ey = bezier_tangent(ay, by, cy, dy, 1.0f);

		while (t < 1.0f) {

			// Calculate point on the bezier curve
			const float px = bezier_point(ax, bx, cx, dx, t);
			const float py = bezier_point(ay, by, cy, dy, t);
			const Vec2 point {px, py};

			// Calculate the tangent at the same point
			const float tx = bezier_tangent(ax, bx, cx, dx, t);
			const float ty = bezier_tangent(ay, by, cy, dy, t);
			const Vec2 tangent = Vec2 {tx, ty}.norm();

			if (draw) {
				slanted_line(last_point, last_tangent, point, tangent);
			}

			last_point = point;
			last_tangent = tangent;
			draw = true;
			t += step;
		}

		// Handle the endpoint
		Vec2 point {dx, dy};
		Vec2 tangent = Vec2 {ex, ey}.norm();
		slanted_line(last_point, last_tangent, point, tangent);

	}

	void Renderer::bezier(Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
		bezier(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y);
	}

	void Renderer::circle(float x, float y, float radius) {
		ellipse(x, y, radius, radius);
	}

	void Renderer::circle(Vec2 p1, float radius) {
		ellipse(p1, radius, radius);
	}

	void Renderer::ellipse(float x, float y, float hrad, float vrad) {
		use(color_pipeline);
		arc(x, y, hrad, vrad, 0, TAU);
	}

	void Renderer::ellipse(Vec2 p1, float hrad, float vrad) {
		use(color_pipeline);
		arc(p1.x, p1.y, hrad, vrad, 0, TAU);
	}

	void Renderer::line(float x1, float y1, float x2, float y2) {
		use(color_pipeline);

		if (!stroke_flag) {
			return;
		}

		float dx = y1 - y2;
		float dy = x2 - x1;
		float dl = sqrt(dx * dx + dy * dy);

		dx /= dl;
		dy /= dl;

		dx *= stroke_width;
		dy *= stroke_width;

		svert(x1 + dx, y1 + dy);
		svert(x2 - dx, y2 - dy);
		svert(x2 + dx, y2 + dy);

		svert(x1 + dx, y1 + dy);
		svert(x1 - dx, y1 - dy);
		svert(x2 - dx, y2 - dy);
	}

	void Renderer::line(Vec2 p1, Vec2 p2) {
		line(p1.x, p1.y, p2.x, p2.y);
	}

	void Renderer::point(float x, float y) {
		circle(x, y, 0);
	}

	void Renderer::point(Vec2 p1) {
		circle(p1.x, p1.y, 0);
	}

	void Renderer::trig(float x1, float y1, float x2, float y2, float x3, float y3) {
		use(color_pipeline);

		if (fill_flag)
		{
			fvert(x1, y1);
			fvert(x2, y2);
			fvert(x3, y3);
		}

		if (stroke_flag)
		{
			Vec2 pa {x1, y1};
			Vec2 pb {x2, y2};
			Vec2 pc {x3, y3};

			drawStrokeSegment(pa, pb, pc);
			drawStrokeSegment(pb, pc, pa);
			drawStrokeSegment(pc, pa, pb);
		}
	}

	void Renderer::trig(Vec2 p1, Vec2 p2, Vec2 p3) {
		trig(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
	}

	void Renderer::quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
		use(color_pipeline);

		if (fill_flag) {
			fvert(x1, y1);
			fvert(x2, y2);
			fvert(x3, y3);

			fvert(x1, y1);
			fvert(x3, y3);
			fvert(x4, y4);
		}

		if (stroke_flag) {
			Vec2 pa {x1, y1};
			Vec2 pb {x2, y2};
			Vec2 pc {x3, y3};
			Vec2 pd {x4, y4};

			drawStrokeSegment(pa, pb, pd);
			drawStrokeSegment(pb, pc, pa);
			drawStrokeSegment(pc, pd, pb);
			drawStrokeSegment(pd, pa, pc);
		}
	}

	void Renderer::quad(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4) {
		quad(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
	}

	void Renderer::square(float x, float y, float e) {
		quad(x, y, x + e, y, x + e, y - e, x, y - e);
	}

	void Renderer::rect(float x, float y, float w, float h, float r1, float r2, float r3, float r4) {
		use(color_pipeline);

		float e = getStrokeWidth();
		float e1 = r1 + e, e2 = r2 + e, e3 = r3 + e, e4 = r4 + e;

		Vec2 par {x + r1,     y - r1 + h};
		Vec2 pbr {x + w - r2, y - r2 + h};
		Vec2 pcr {x + w - r3, y + r3};
		Vec2 pdr {x + r4,     y + r4};

		arc(par.x, par.y, r1, r1, rad(180), -HALF_PI);
		arc(pbr.x, pbr.y, r2, r2, rad(90), -HALF_PI);
		arc(pcr.x, pcr.y, r3, r3, rad(0), -HALF_PI);
		arc(pdr.x, pdr.y, r4, r4, rad(270), -HALF_PI);

		if (fill_flag) {

			// main rect body
			drawFillQuad(par.x, par.y, pbr.x, pbr.y, pcr.x, pcr.y, pdr.x, pdr.y);

			// beveled walls
			drawFillQuad(par.x, par.y, pdr.x, pdr.y, pdr.x - r4, pdr.y, par.x - r1, par.y);
			drawFillQuad(pbr.x, pbr.y, pcr.x, pcr.y, pcr.x + r3, pcr.y, pbr.x + r2, pbr.y);
			drawFillQuad(pcr.x, pcr.y, pdr.x, pdr.y, pdr.x, pdr.y - r4, pcr.x, pcr.y - r3);
			drawFillQuad(par.x, par.y, pbr.x, pbr.y, pbr.x, pbr.y + r2, par.x, par.y + r1);

		}

		if (stroke_flag) {

			// beveled stroke
			drawStrokeQuad(pdr.x - r4, pdr.y, par.x - r1, par.y, par.x - e1, par.y, pdr.x - e4, pdr.y);
			drawStrokeQuad(pcr.x + r3, pcr.y, pbr.x + r2, pbr.y, pbr.x + e2, pbr.y, pcr.x + e3, pcr.y);
			drawStrokeQuad(pdr.x, pdr.y - r4, pcr.x, pcr.y - r3, pcr.x, pcr.y - e3, pdr.x, pdr.y - e4);
			drawStrokeQuad(pbr.x, pbr.y + r2, par.x, par.y + r1, par.x, par.y + e1, pbr.x, pbr.y + e2);

		}

	}

	void Renderer::rect(float x, float y, float w, float h, float r) {
		rect(x, y, w, h, r, r, r, r);
	}

	void Renderer::image(float x, float y, float w, float h) {
		use(image_pipeline);

		ivert(x, y - h, bx, ey);
		ivert(x, y, bx, by);
		ivert(x + w , y, ex, by);

		ivert(x, y - h, bx, ey);
		ivert(x + w , y, ex, by);
		ivert(x + w, y - h, ex, ey);
	}

	void Renderer::image(float x, float y) {
		use(image_pipeline);

		image(x, y, tw, th);
	}

	void Renderer::text(float x, float y, const std::string& str) {
		use(fonts_pipeline);

		Font& font = (Font&) getTexture();
		int unicode = 0;
		int prev = 0;
		int offset = 0;

		while (true) {
			prev = unicode;
			unicode = next_unicode(str.c_str(), &offset);

			if (unicode == 0) {
				break;
			}

			GlyphQuad q = font.getBakedQuad(&x, &y, font.getScaleForSize(text_size), unicode, prev, [this] () {
				flush();
			});

			ivert(q.x0, q.y1, q.s0, q.t1);
			ivert(q.x0, q.y0, q.s0, q.t0);
			ivert(q.x1, q.y0, q.s1, q.t0);

			ivert(q.x0, q.y1, q.s0, q.t1);
			ivert(q.x1, q.y0, q.s1, q.t0);
			ivert(q.x1, q.y1, q.s1, q.t1);
		}
	}

}