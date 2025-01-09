#pragma once

#include "texture.hpp"
#include "atlas.hpp"

namespace msdfgen {
	struct FontHandle;
}

namespace plgl {

	struct GlyphInfo {
		double x0, y0, x1, y1;
		double xoff, yoff, advance;
	};

	struct GlyphQuad {
		float x0, y0, s0, t0;
		float x1, y1, s1, t1;
	};

	class Font : public PixelBuffer {

		public:

			static constexpr int resolution = 64;

		private:

			float base;
			msdfgen::FontHandle* font;
			Atlas atlas;
			ankerl::unordered_dense::map<int, GlyphInfo> cdata;

			bool loadUnicode(msdfgen::FontHandle* font, uint32_t unicode, float scale, float range, const std::function<void()>& on_resize);

		public:

			Font(const char* path, int weight = 400);

			float getScaleForSize(float size) const;
			GlyphQuad getBakedQuad(float* x, float* y, float scale, int code, int prev, const std::function<void()>& on_resize);

			void use() const final;
			int handle() const final;
			int width() const final;
			int height() const final;
	};

}
