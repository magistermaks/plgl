#pragma once

#include "texture.hpp"

namespace plgl {

	class Font : public Texture {

		public:

			float base;
			stbtt_bakedchar cdata[96]; // ASCII 32 (space) .. 126 (~) is 95 glyphs
			float lineGap;

		public:

			Font(const char* path, float height);

			float getScaleForSize(float size) const;
			float getFontLineGap() const;
			stbtt_aligned_quad getBakedQuad(float* x, float* y, int code, float scale) const;

	};

}
