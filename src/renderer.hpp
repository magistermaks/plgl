#pragma once

#include "pipeline.hpp"
#include "disabled.hpp"
#include "quality.hpp"
#include "util.hpp"
#include "font.hpp"
#include "color.hpp"
#include "math.hpp"

namespace plgl {

	class Renderer {

		private:

			// currently used pipeline
			Pipeline* pipeline = nullptr;

			Pipeline* color_pipeline = new Pipeline {Pipeline::getColorShader(), nullptr};
			Pipeline* image_pipeline = nullptr;
			Pipeline* fonts_pipeline = nullptr;

			std::unordered_map<GLuint, Pipeline> textures;

			float text_size;
			float bx, by, ex, ey;
			float draw_width;
			float draw_quality;

			// stroke settings
			bool stroke_flag;
			float sr, sg, sb, sa;

			// fill settings
			bool fill_flag;
			float fr, fg, fb, fa;

			// tint settings
			float tr, tg, tb, ta;

		private:

			void use(Pipeline* pipeline) {
				if (this->pipeline != pipeline) {
					flush();
					this->pipeline = pipeline;
				}
			}

			Texture& getTexture() {
				return *pipeline->texture;
			}

			void svert(float x, float y) {
				pipeline->buffer.vertex(x, y, sr, sg, sb, sa);
			}

			void fvert(float x, float y) {
				pipeline->buffer.vertex(x, y, fr, fg, fb, fa);
			}

			void ivert(float x, float y, float u, float v) {
				pipeline->buffer.vertex(x, y, u, v, tr, tg, tb, ta);
			}

			void registerTexture(Texture& texture, Shader& shader) {
				textures.emplace(
					std::piecewise_construct, 
					std::forward_as_tuple(texture.getTid()), 
					std::forward_as_tuple(shader, &texture)
				);
			}

		public:

			Renderer() {
				quality(MEDIUM);
				fill(255, 255, 255);
				tint(255, 255, 255);
				stroke(0, 0, 0);
				weight(1);
				size(20);
			}

			~Renderer() {
				delete color_pipeline;
			}

			void stroke(Disabled disabled) {
				this->stroke_flag = false;
			}

			void stroke(float r, float g, float b, float a = 255) {
				this->stroke_flag = true;
				this->sr = impl::norm(r);
				this->sg = impl::norm(g);
				this->sb = impl::norm(b);
				this->sa = impl::norm(a);
			}

			void stroke(const impl::Color& color) {
				RGBA rgba = color.as_rgba();
				stroke(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());	
			}

			void weight(float w) {
				this->draw_width = w;
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
				this->fr = impl::norm(r);
				this->fg = impl::norm(g);
				this->fb = impl::norm(b);
				this->fa = impl::norm(a);
			}

			void fill(const impl::Color& color) {
				RGBA rgba = color.as_rgba();
				fill(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());	
			}

			void tint(Disabled disabled) {
				tint(255, 255, 255);
			}

			void tint(float r, float g, float b, float a = 255) {
				this->tr = impl::norm(r);
				this->tg = impl::norm(g);
				this->tb = impl::norm(b);
				this->ta = impl::norm(a);
			}

			void tint(const impl::Color& color) {
				RGBA rgba = color.as_rgba();
				tint(rgba.red(), rgba.green(), rgba.blue(), rgba.alpha());	
			}

			void texture(Texture& t, float bx, float by, float ex, float ey) {
				GLuint tid = t.getTid();
				float w = t.getWidth();
				float h = t.getHeight();

				if (!textures.count(tid)) {
					registerTexture(t, Pipeline::getImageShader());
				}

				this->image_pipeline = &textures.at(tid);

				this->bx = std::min(bx, w) / w;
				this->by = std::min(by, h) / h;
				this->ex = std::min(ex, w) / w;
				this->ey = std::min(ey, h) / h;
			}

			void texture(Texture& t) {
				texture(t, 0, 0, t.getWidth(), t.getHeight());
			}

			void font(Font& f) {
				GLuint tid = f.getTid();

				if (!textures.count(tid)) {
					registerTexture(f, Pipeline::getFontShader());
				}

				this->fonts_pipeline = &textures.at(tid);
			}

			void size(float s) {
				this->text_size = s;
			}
			
			void flush() {
				if (this->pipeline != nullptr) {
					this->pipeline->flush();
				}
			}

		public:

			void circle(float x, float y, float radius) {
				ellipse(x, y, radius, radius);
			}

			void ellipse(float x, float y, float hrad, float vrad) {
				use(color_pipeline);

				float extension = stroke_flag ? draw_width : 0;
				float herad = hrad + extension;
				float verad = vrad + extension;
				float extent = std::max(herad, verad);

				float correctness = 1 - draw_quality / extent;
				int sides = std::max(3, (int) ceil(TAU / acos(2 * correctness * correctness - 1)));
				float step = TAU / sides;

				for (int i = 0; i < sides; i ++) {
					float ax = x + hrad * cos(step * i);
					float ay = y + vrad * sin(step * i);

					float bx = x + hrad * cos(step * (i + 1));
					float by = y + vrad * sin(step * (i + 1));
					
					if (fill_flag) {
						fvert(x, y);
						fvert(ax, ay);
						fvert(bx, by);
					}

					if (stroke_flag) {
						float cx = x + herad * cos(step * i);
						float cy = y + verad * sin(step * i);

						float dx = x + herad * cos(step * (i + 1));
						float dy = y + verad * sin(step * (i + 1));

						svert(ax, ay);
						svert(cx, cy);
						svert(dx, dy);

						svert(ax, ay);
						svert(dx, dy);
						svert(bx, by);
					}
				}
			}

			void line(float x1, float y1, float x2, float y2) {
				use(color_pipeline);

				if (!stroke_flag) {
					return;
				}		

				float a = -1.0f / ((y1 - y2)/(x1 - x2));
				float d = sqrt((4 * draw_width * draw_width) / (1 + a * a));
			  
				float ax1 = (2 * x1 + d) / 2;
				float ax2 = (2 * x1 - d) / 2;
				float bx1 = (2 * x2 + d) / 2;
				float bx2 = (2 * x2 - d) / 2;
			  
				float ay1 = a * (ax1 - x1) + y1;
				float ay2 = a * (ax2 - x1) + y1;
				float by1 = a * (bx1 - x2) + y2;
				float by2 = a * (bx2 - x2) + y2;
			  
				if (d == 0) {
					ax1 = ax2 = x1;
					ay1 = y1 - draw_width;
					ay2 = y1 + draw_width;
				
					bx1 = bx2 = x2;
					by1 = y2 - draw_width;
					by2 = y2 + draw_width;
				}

				svert(ax2, ay2);
				svert(bx2, by2);
				svert(ax1, ay1);

				svert(bx2, by2);
				svert(bx1, by1);
				svert(ax1, ay1);

			}

			void point(float x, float y) {
				circle(x, y, 0);
			}

			void trig(float x1, float y1, float x2, float y2, float x3, float y3) {
				use(color_pipeline);				

				if (fill_flag) {
					fvert(x1, y1);
					fvert(x2, y2);
					fvert(x3, y3);
				}

				if (stroke_flag) {
					
					// v1 = AB / |AB| * f
					float vd1 = dist(x1, y1, x2, y2) / draw_width;
					float vx1 = (x2 - x1) / vd1;
					float vy1 = (y2 - y1) / vd1;
					
					// v2 = BC / |BC| * f
					float vd2 = dist(x2, y2, x3, y3) / draw_width;
					float vx2 = (x3 - x2) / vd2;
					float vy2 = (y3 - y2) / vd2;
					
					// v3 = CA / |CA| * f
					float vd3 = dist(x3, y3, x1, y1) / draw_width;
					float vx3 = (x1 - x3) / vd3;
					float vy3 = (y1 - y3) / vd3;
					
					// two points near (x1, y1)
					float pax1 = x1 - vy1;
					float pay1 = y1 + vx1;
					float pax2 = x1 - vy3;
					float pay2 = y1 + vx3;
					svert(x1, y1); // draw corner
					svert(pax1, pay1);
					svert(pax2, pay2);
					
					// two points near (x2, y2)
					float pbx1 = x2 - vy1;
					float pby1 = y2 + vx1;
					float pbx2 = x2 - vy2;
					float pby2 = y2 + vx2;
					svert(x2, y2); // draw corner
					svert(pbx1, pby1);
					svert(pbx2, pby2);
				  
					// two points near (x3, y3)
					float pcx1 = x3 - vy2;
					float pcy1 = y3 + vx2;
					float pcx2 = x3 - vy3;
					float pcy2 = y3 + vx3;
					svert(x3, y3); // draw corner
					svert(pcx1, pcy1);
					svert(pcx2, pcy2);

					// stroke (x1, y1) to (x2, y2)
					svert(x1, y1);
					svert(pax1, pay1);
					svert(pbx1, pby1);
					svert(x1, y1);
					svert(pbx1, pby1);
					svert(x2, y2);

					// stroke (x2, y2) to (x3, y3)
					svert(x2, y2);
					svert(x3, y3);
					svert(pbx2, pby2);
					svert(pbx2, pby2);
					svert(x3, y3);
					svert(pcx1, pcy1);

					// stroke (x3, y3) to (x1, y1)
					svert(x3, y3);
					svert(x1, y1);
					svert(pax2, pay2);
					svert(x3, x3);
					svert(pax2, pay2);
					svert(pcx2, pcy2);

					// external intersection near A
					float adiv = 1.0f / (vx1 * vy3 - vy1 * vx3);
					float a12 = pay1 * (pax1 + vx1) - pax1 * (pay1 + vy1);
					float a34 = pax2 * (pay2 + vy3) - pay2 * (pax2 + vx3);
					float apx = (a12 * vx3 + vx1 * a34) * adiv;
					float apy = (a12 * vy3 + vy1 * a34) * adiv;
					
					// external intersection near B
					float bdiv = 1.0f / (vx1 * vy2 - vy1 * vx2);
					float b12 = pby1 * (pbx1 + vx1) - pbx1 * (pby1 + vy1);
					float b34 = pbx2 * (pby2 + vy2) - pby2 * (pbx2 + vx2);
					float bpx = (b12 * vx2 + vx1 * b34) * bdiv;
					float bpy = (b12 * vy2 + vy1 * b34) * bdiv;
					
					// external intersection near C
					float cdiv = 1.0f / (vx2 * vy3 - vy2 * vx3);
					float c12 = pcy1 * (pcx1 + vx2) - pcx1 * (pcy1 + vy2);
					float c34 = pcx2 * (pcy2 + vy3) - pcy2 * (pcx2 + vx3);
					float cpx = (c12 * vx3 + vx2 * c34) * cdiv;
					float cpy = (c12 * vy3 + vy2 * c34) * cdiv;

					svert(apx, apy); // draw external corner A
					svert(pax1, pay1);
					svert(pax2, pay2);
 
					svert(bpx, bpy); // draw external corner B
					svert(pbx1, pby1);
					svert(pbx2, pby2);

					svert(cpx, cpy); // draw external corner C
					svert(pcx1, pcy1);
					svert(pcx2, pcy2);

				}

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
				image(x, y, getTexture().getWidth(), getTexture().getHeight());

			}

			void text(float x, float y, const std::string& str) {

				use(fonts_pipeline);
				Font& font = (Font&) getTexture();

				for (char chr : str) {
					stbtt_aligned_quad q = font.getBakedQuad(&x, &y, chr, font.getScaleForSize(text_size));

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

	};

}
