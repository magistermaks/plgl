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

	struct BakedGlyph {
		float x0, y0, s0, t0; // top-left
		float x1, y1, s1, t1; // bottom-right
	};

	class Font : public Texture {

		private:

			bool loadUnicode(msdfgen::FontHandle* font, uint32_t unicode, int size, float scale, float range, bool* flush);

			bool modified = false;
			Atlas atlas;

		public:

			float base;
			float lineGap;
			msdfgen::FontHandle* handle;
			std::unordered_map<int, GlyphInfo> cdata;

		public:

			Font(const char* path, float height);

			float getScaleForSize(float size) const;
			float getFontLineGap() const;
			stbtt_aligned_quad getBakedQuad(bool* flush, float* x, float* y, int code, float scale, int prev = 0);

			void prepare();

	};

}
