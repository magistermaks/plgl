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

			// value in range [0, 1], lower is better
			float draw_quality;

			void slanted_line(Vec2 p1, Vec2 d1, Vec2 p2, Vec2 d2);

		public:

			Renderer();

			void stroke(Disabled disabled);

			void stroke(float r, float g, float b, float a = 255);

			void stroke(const Color& color);

			void weight(float w);

			/// configures the max error allowed
			void quality(Quality q);

			void fill(Disabled disabled);

			void fill(float r, float g, float b, float a = 255);

			void fill(const Color& color);

			void tint(Disabled disabled);

			void tint(float r, float g, float b, float a = 255);

			void tint(const Color& color);

			void texture(Sprite& sprite);

			void texture(Texture& t, float bx, float by, float ex, float ey);

			void texture(Texture& t);

			void font(Font& f);

			void size(float s);

			void arc(float x, float y, float hrad, float vrad, float start, float angle, ArcMode mode = OPEN_PIE);

		public:

			float bezier_point(float a, float b, float c, float d, float t);

			float bezier_tangent(float a, float b, float c, float d, float t);

			void bezier(float ax, float ay, float bx, float by, float cx, float cy, float dx, float dy);

			void bezier(Vec2 a, Vec2 b, Vec2 c, Vec2 d);

			void circle(float x, float y, float radius);

			void circle(Vec2 p1, float radius);

			void ellipse(float x, float y, float hrad, float vrad);

			void ellipse(Vec2 p1, float hrad, float vrad);

			void line(float x1, float y1, float x2, float y2);

			void line(Vec2 p1, Vec2 p2);

			void point(float x, float y);

			void point(Vec2 p1);

			void trig(float x1, float y1, float x2, float y2, float x3, float y3);

			void trig(Vec2 p1, Vec2 p2, Vec2 p3);

			void quad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

			void quad(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4);

			void square(float x, float y, float e);

			void rect(float x, float y, float w, float h, float r1, float r2, float r3, float r4);

			void rect(float x, float y, float w, float h, float r);

			void image(float x, float y, float w, float h);

			void image(float x, float y);

			void text(float x, float y, const std::string& str);

			template<class... Args>
			void textf(float x, float y, const std::string& str, Args&&... args) {
				text(x, y, format(str, args...));
			}

		public:

			int grid_size = 0;
			float padding_size;
			float radius_size;
			int gui_box_w, gui_box_h;

			inline void gui_begin(int grid) {
				grid_size = grid;
			}

			inline void gui_padding(float padding) {
				padding_size = padding;
			}

			inline void gui_radius(float radius) {
				radius_size = radius;
			}

			inline void gui_sizing(int w, int h) {
				gui_box_w = w;
				gui_box_h = h;
			}

			inline bool gui_button(int x, int y, const std::string& text) {

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

			inline bool gui_panel(int x, int y) {

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
