#pragma once

#include "pipeline.hpp"
#include "disabled.hpp"
#include "quality.hpp"
#include "util.hpp"
#include "font.hpp"
#include "color.hpp"
#include "math.hpp"
#include "basic.hpp"
#include "arc.hpp"
#include "atlas.hpp"
#include "utf8.hpp"

namespace plgl {

	class Renderer : public impl::BasicRenderer {

		private:

			int tw, th;
			float text_size;
			float bx, by, ex, ey;
			float draw_quality;

		public:

			Renderer() {
				quality(MEDIUM);
				fill(255, 255, 255);
				tint(255, 255, 255);
				stroke(0, 0, 0);
				weight(1);
				size(20);
			}

			void stroke(Disabled disabled) {
				this->stroke_flag = false;
			}

			void stroke(float r, float g, float b, float a = 255) {
				this->stroke_flag = true;
				this->sr = r;
				this->sg = g;
				this->sb = b;
				this->sa = a;
			}

			void stroke(const Color& color) {
				RGBA rgba = color.as_rgba();
				stroke(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());
			}

			void weight(float w) {
				this->stroke_width = w;
			}

			/// configures the max error allowed
			void quality(Quality q) {
				this->draw_quality = ((float) q) * 0.1f;
			}

			void fill(Disabled disabled) {
				this->fill_flag = false;
			}

			void fill(float r, float g, float b, float a = 255) {
				this->fill_flag = true;
				this->fr = r;
				this->fg = g;
				this->fb = b;
				this->fa = a;
			}

			void fill(const Color& color) {
				RGBA rgba = color.as_rgba();
				fill(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());
			}

			void tint(Disabled disabled) {
				tint(255, 255, 255);
			}

			void tint(float r, float g, float b, float a = 255) {
				this->tr = r;
				this->tg = g;
				this->tb = b;
				this->ta = a;
			}

			void tint(const Color& color) {
				RGBA rgba = color.as_rgba();
				tint(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());
			}

			void texture(Sprite& sprite) {
				texture(*sprite.texture, sprite.x, sprite.y, sprite.x + sprite.w, sprite.y + sprite.h);
			}

			void texture(Texture& t, float bx, float by, float ex, float ey) {
				useTexture(t);

				float w = t.getWidth();
				float h = t.getHeight();

				this->bx = std::min(bx, w) / w;
				this->by = std::min(by, h) / h;
				this->ex = std::min(ex, w) / w;
				this->ey = std::min(ey, h) / h;

				this->tw = std::abs(bx - ex);
				this->th = std::abs(by - ey);
			}

			void texture(Texture& t) {
				texture(t, 0, 0, t.getWidth(), t.getHeight());

				this->tw = t.getWidth();
				this->th = t.getHeight();
			}

			void font(Font& f) {
				useFont(f);
			}

			void size(float s) {
				this->text_size = s;
			}

			void arc(float x, float y, float hrad, float vrad, float start, float angle, ArcMode mode = OPEN_PIE) {

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

		public:

			void circle(float x, float y, float radius) {
				ellipse(x, y, radius, radius);
			}

			void circle(Vec2 p1, float radius) {
				ellipse(p1, radius, radius);
			}

			void ellipse(float x, float y, float hrad, float vrad) {
				use(color_pipeline);
				arc(x, y, hrad, vrad, 0, TAU);
			}

			void ellipse(Vec2 p1, float hrad, float vrad) {
				use(color_pipeline);
				arc(p1.x, p1.y, hrad, vrad, 0, TAU);
			}

			void line(float x1, float y1, float x2, float y2) {
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

			void line(Vec2 p1, Vec2 p2) {
				line(p1.x, p1.y, p2.x, p2.y);
			}

			void point(float x, float y) {
				circle(x, y, 0);
			}

			void point(Vec2 p1) {
				circle(p1.x, p1.y, 0);
			}

			void trig(float x1, float y1, float x2, float y2, float x3, float y3) {
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

			void trig(Vec2 p1, Vec2 p2, Vec2 p3) {
				trig(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
			}

			void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
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

			void quad(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4) {
				quad(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
			}

			void square(float x, float y, float e) {
				quad(x, y, x + e, y, x + e, y - e, x, y - e);
			}

			void rect(float x, float y, float w, float h, float r1, float r2, float r3, float r4) {
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

			void rect(float x, float y, float w, float h, float r) {
				rect(x, y, w, h, r, r, r, r);
			}

			void image(float x, float y, float w, float h) {
				use(image_pipeline);

				ivert(x, y - h, bx, ey);
				ivert(x, y, bx, by);
				ivert(x + w , y, ex, by);

				ivert(x, y - h, bx, ey);
				ivert(x + w , y, ex, by);
				ivert(x + w, y - h, ex, ey);
			}

			void image(float x, float y) {
				use(image_pipeline);

				image(x, y, tw, th);
			}

			void text(float x, float y, const std::string& str) {
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

			template<class... Args>
			void textf(float x, float y, const std::string& str, Args&&... args) {
				text(x, y, format(str, args...));
			}

		public:

			int grid_size = 0;
			float padding_size;
			float radius_size;
			int gui_box_w, gui_box_h;

			void gui_begin(int grid) {
				grid_size = grid;
			}

			void gui_padding(float padding) {
				padding_size = padding;
			}

			void gui_radius(float radius) {
				radius_size = radius;
			}

			void gui_sizing(int w, int h) {
				gui_box_w = w;
				gui_box_h = h;
			}

			bool gui_button(int x, int y, const std::string& text) {

				stroke(OFF);
				fill(90, 90, 90);

				int text_size = 30;

				int rx = 100 + x * grid_size + padding_size;
				int ry = 100 + y * grid_size + padding_size;
				int rw = grid_size * gui_box_w - 2 * padding_size;
				int rh = grid_size * gui_box_h - 2 * padding_size;

				if (mouse_x > rx && mouse_y > ry && mouse_x < rx + rw && mouse_y < ry + rh) {
					if (mouse_pressed) {
						stroke(80, 80, 130);
						weight(2);
						fill(150, 150, 150);
					} else {
						stroke(80, 80, 130);
						weight(2);
						fill(110, 110, 110);
					}
				}

				rect(rx, ry, rw, rh, radius_size);

				size(text_size);
				stroke(0, 0, 0);
				textf(rx, ry + rh / 2, "ABCdef!");

			}

			bool gui_panel(int x, int y) {

				stroke(OFF);
				fill(255, 255, 255);

				int rx = 100 + x * grid_size + padding_size;
				int ry = 100 + y * grid_size + padding_size;
				int rw = grid_size * gui_box_w - 2 * padding_size;
				int rh = grid_size * gui_box_h - 2 * padding_size;

				rect(rx, ry, rw, rh, radius_size);

			}

	};

}
