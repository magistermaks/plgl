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

	};

}
