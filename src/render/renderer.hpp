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

			// draws the line stroke between p1-p2, and the angle stroke at p2
			void stroke_vertex(const Vec2& pa, const Vec2& pb, const Vec2& pc) {

				Vec2 v1 = (pb - pa).normalized() * draw_width;
				Vec2 v3 = (pa - pc).normalized() * draw_width;

				Vec2 pa1 = pa + v1.perpendicular();
				Vec2 pa2 = pa + v3.perpendicular();
				Vec2 pc2 = pc + v3.perpendicular();

				// draw corner near A
				svert(pa.x, pa.y);
				svert(pa1.x, pa1.y);
				svert(pa2.x, pa2.y);

				// stroke C to A
				svert(pc.x, pc.y);
				svert(pa.x, pa.y);
				svert(pa2.x, pa2.y);
				svert(pc.x, pc.y);
				svert(pa2.x, pa2.y);
				svert(pc2.x, pc2.y);
	
				// external intersection near A
				float adiv = 1.0f / (v1.x * v3.y - v1.y * v3.x);
				float a12 = pa1.y * (pa1.x + v1.x) - pa1.x * (pa1.y + v1.y);
				float a34 = pa2.x * (pa2.y + v3.y) - pa2.y * (pa2.x + v3.x);
				float apx = (a12 * v3.x + v1.x * a34) * adiv;
				float apy = (a12 * v3.y + v1.y * a34) * adiv;

				// draw external corner A
				svert(apx, apy);
				svert(pa1.x, pa1.y);
				svert(pa2.x, pa2.y);

			}

			inline void fill_quad_vertices(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
				fvert(x1, y1);
				fvert(x2, y2);
				fvert(x3, y3);
				fvert(x1, y1);
				fvert(x3, y3);
				fvert(x4, y4);
			}

			inline void stroke_quad_vertices(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
				svert(x1, y1);
				svert(x2, y2);
				svert(x3, y3);
				svert(x1, y1);
				svert(x3, y3);
				svert(x4, y4);
			}

			void arc(float x, float y, float hrad, float vrad, float angle, float start) {
				
				float extension = stroke_flag ? draw_width : 0;
				float herad = hrad + extension;
				float verad = vrad + extension;
				float extent = std::max(herad, verad);

				float correctness = 1 - draw_quality / extent;
				int sides = std::max(3, (int) ceil(TAU / acos(2 * correctness * correctness - 1)));
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

			}

			

		public:

			void circle(float x, float y, float radius) {
				ellipse(x, y, radius, radius);
			}

			void ellipse(float x, float y, float hrad, float vrad) {
				use(color_pipeline);
				arc(x, y, hrad, vrad, TAU, 0);
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

					Vec2 pa {x1, y1};
					Vec2 pb {x2, y2};
					Vec2 pc {x3, y3};

					stroke_vertex(pa, pb, pc);
					stroke_vertex(pb, pc, pa);
					stroke_vertex(pc, pa, pb);
					
				}
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

					stroke_vertex(pa, pb, pd);
					stroke_vertex(pb, pc, pa);
					stroke_vertex(pc, pd, pb);	
					stroke_vertex(pd, pa, pc);
				}
			}

			void square(float x, float y, float e) {
				quad(x, y, x + e, y, x + e, y - e, x, y - e);
			}

			void rect(float x, float y, float w, float h, float r1, float r2, float r3, float r4) {
				use(color_pipeline);

				float e = (stroke_flag ? draw_width : 0);
				float e1 = r1 + e, e2 = r2 + e, e3 = r3 + e, e4 = r4 + e;

				Vec2 par {x + r1,     y - r1};
				Vec2 pbr {x + w - r2, y - r2};
				Vec2 pcr {x + w - r3, y - h + r3};
				Vec2 pdr {x + r4,     y - h + r4};

				arc(par.x, par.y, r1, r1, -HALF_PI, rad(180));
				arc(pbr.x, pbr.y, r2, r2, -HALF_PI, rad(90));
				arc(pcr.x, pcr.y, r3, r3, -HALF_PI, rad(0));
				arc(pdr.x, pdr.y, r4, r4, -HALF_PI, rad(270));

				if (fill_flag) {
					
					// main rect body
					fill_quad_vertices(par.x, par.y, pbr.x, pbr.y, pcr.x, pcr.y, pdr.x, pdr.y);

					// beveled walls
					fill_quad_vertices(par.x, par.y, pdr.x, pdr.y, pdr.x - r4, pdr.y, par.x - r1, par.y);
					fill_quad_vertices(pbr.x, pbr.y, pcr.x, pcr.y, pcr.x + r3, pcr.y, pbr.x + r2, pbr.y);
					fill_quad_vertices(pcr.x, pcr.y, pdr.x, pdr.y, pdr.x, pdr.y - r4, pcr.x, pcr.y - r3);
					fill_quad_vertices(par.x, par.y, pbr.x, pbr.y, pbr.x, pbr.y + r2, par.x, par.y + r1);

				}

				if (stroke_flag) {

					// beveled stroke
					stroke_quad_vertices(pdr.x - r4, pdr.y, par.x - r1, par.y, par.x - e1, par.y, pdr.x - e4, pdr.y);
					stroke_quad_vertices(pcr.x + r3, pcr.y, pbr.x + r2, pbr.y, pbr.x + e2, pbr.y, pcr.x + e3, pcr.y);
					stroke_quad_vertices(pdr.x, pdr.y - r4, pcr.x, pcr.y - r3, pcr.x, pcr.y - e3, pdr.x, pdr.y - e4);
					stroke_quad_vertices(pbr.x, pbr.y + r2, par.x, par.y + r1, par.x, par.y + e1, pbr.x, pbr.y + e2);

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
