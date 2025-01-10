#pragma once

#include "pipeline.hpp"
#include "disabled.hpp"
#include "quality.hpp"
#include "util.hpp"
#include "font.hpp"
#include "color.hpp"
#include "math.hpp"

namespace plgl::impl {

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

			virtual ~BasicRenderer();

			void use(Pipeline* pipeline);

			void svert(float x, float y);
			void fvert(float x, float y);
			void ivert(float x, float y, float u, float v);

			float getStrokeWidth();
			PixelBuffer& getTexture();
			void registerTexture(PixelBuffer& texture, Shader& shader);
			void drawStrokeSegment(const Vec2& pa, const Vec2& pb, const Vec2& pc);
			void drawFillQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
			void drawStrokeQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

		public:

			void useTexture(Texture& t);
			void useFont(Font& f);
			void flush();
			void clip(float x1, float y1, float x2, float y2);
			void clip(Disabled disabled);

	};

}
