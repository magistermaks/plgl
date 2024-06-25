#pragma once

#include "pipeline.hpp"
#include "disabled.hpp"
#include "quality.hpp"
#include "util.hpp"
#include "font.hpp"
#include "color.hpp"
#include "math.hpp"

namespace plgl {

	namespace impl {

		class BasicRenderer {

			private:

				// currently used pipeline
				Pipeline* pipeline = nullptr;

			protected:

				Pipeline* color_pipeline = new Pipeline {Pipeline::getColorShader(), nullptr};
				Pipeline* image_pipeline = nullptr;
				Pipeline* fonts_pipeline = nullptr;

				std::unordered_map<GLuint, Pipeline> textures;

				float sr, sg, sb, sa; // stroke
				float fr, fg, fb, fa; // fill
				float tr, tg, tb, ta; // tint

				float stroke_width;
				bool stroke_flag;
				bool fill_flag;

			protected:

				virtual ~BasicRenderer() {
					delete color_pipeline;
				}

				void use(Pipeline* pipeline) {
					if (this->pipeline != pipeline) {
						flush();
						this->pipeline = pipeline;
					}
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

				float getStrokeWidth() {
					return stroke_flag ? stroke_width : 0;
				}

				Texture& getTexture() {
					return *pipeline->texture;
				}

				void registerTexture(Texture& texture, Shader& shader) {
					textures.emplace(
						std::piecewise_construct,
						std::forward_as_tuple(texture.getTid()),
						std::forward_as_tuple(shader, &texture)
					);
				}

				void drawStrokeSegment(const Vec2& pa, const Vec2& pb, const Vec2& pc) {

					Vec2 v1 = (pb - pa).norm() * stroke_width;
					Vec2 v3 = (pa - pc).norm() * stroke_width;

					Vec2 pa1 = pa + v1.perp();
					Vec2 pa2 = pa + v3.perp();
					Vec2 pc2 = pc + v3.perp();

					// external intersection near A
					float adiv = 1.0f / (v1.x * v3.y - v1.y * v3.x);
					float a12 = pa1.y * (pa1.x + v1.x) - pa1.x * (pa1.y + v1.y);
					float a34 = pa2.x * (pa2.y + v3.y) - pa2.y * (pa2.x + v3.x);
					float apx = (a12 * v3.x + v1.x * a34) * adiv;
					float apy = (a12 * v3.y + v1.y * a34) * adiv;

					// draw corner near A
					svert(apx, apy);
					svert(pa1.x, pa1.y);
					svert(pa.x, pa.y);

					// inner stroke A to C
					svert(apx, apy);
					svert(pa.x, pa.y);
					svert(pc.x, pc.y);

					// outer stroke A to C
					svert(apx, apy);
					svert(pc.x, pc.y);
					svert(pc2.x, pc2.y);

				}

				inline void drawFillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
					fvert(x1, y1);
					fvert(x2, y2);
					fvert(x3, y3);
					fvert(x1, y1);
					fvert(x3, y3);
					fvert(x4, y4);
				}

				inline void drawStrokeQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
					svert(x1, y1);
					svert(x2, y2);
					svert(x3, y3);
					svert(x1, y1);
					svert(x3, y3);
					svert(x4, y4);
				}

			public:

				void useTexture(Texture& t) {
					GLuint tid = t.getTid();

					if (!textures.count(tid)) {
						registerTexture(t, Pipeline::getImageShader());
					}

					this->image_pipeline = &textures.at(tid);
				}

				void useFont(Font& f) {
					GLuint tid = f.getTid();

					if (!textures.count(tid)) {
						registerTexture(f, Pipeline::getFontShader());
					}

					this->fonts_pipeline = &textures.at(tid);
				}

				void flush() {
					if (this->pipeline != nullptr) {
						this->pipeline->flush();
					}
				}

				void clip(float x1, float y1, float x2, float y2) {

					float x_min = std::min(x1, x2);
					float y_min = std::min(y1, y2);
					float x_max = std::max(x1, x2);
					float y_max = std::max(y1, y2);

					// A (x_min, y_min)
					// |
					// X--B (x_max, y_max)
					//
					// then, X (x_min, y_max)
					//       w = |XB|
					//       h = |XA|

					float w = x_max - x_min;
					float h = y_max - y_min;

					flush();
					glScissor((int) x_min, (int) (height - y_max), (int) w, (int) h);
				}

				void clip(Disabled disabled) {
					clip(0, 0, width, height);
				}

		};

	}

}
