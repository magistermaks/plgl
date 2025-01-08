#pragma once

#include "texture.hpp"

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

		public:

			float base;
			GlyphInfo cdata[96]; // ASCII 32 (space) .. 126 (~) is 95 glyphs
			float lineGap;
			msdfgen::FontHandle* handle;

		public:

			Font(const char* path, float height);

			float getScaleForSize(float size) const;
			float getFontLineGap() const;
			stbtt_aligned_quad getBakedQuad(float* x, float* y, int code, float scale, int prev = 0) const;

	};

}
