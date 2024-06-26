#pragma once

#include "texture.hpp"

namespace plgl {

	class Font : public Texture {

		private:

			float base;
			stbtt_bakedchar cdata[96]; // ASCII 32 (space) .. 126 (~) is 95 glyphs

		public:

			Font(const char* path, float height) : Texture() {

				// open file
				std::ifstream file(path, std::ios::binary | std::ios::ate);
				unsigned int length = file.tellg();
				file.seekg(0, std::ios::beg);

				// prepare buffer
				std::vector<char> buffer;
				buffer.reserve(length);

				// load file into buffer
				if (!file.read(buffer.data(), length)) {
					impl::fatal("Unable to load font: '%s'!", path);
				}

				this->base = height;
				int size = 256;
				bool build = false;

				while (!build) {
					size *= 2;

					unsigned char* bitmap = new unsigned char[size * size];
					int magic = stbtt_BakeFontBitmap((unsigned char*) buffer.data(), 0, height, bitmap, size, size, 32, 96, cdata);
					//printf("PLGL: Baked font '%s' into %dx%d bitmap, magic=%d\n", path, size, size, magic);

					if (magic > 0) {
						upload(bitmap, size, size, 1);
						build = true;
					}

					delete[] bitmap;
				}
			}

			float getScaleForSize(float size) {
				return size / base;
			}

			stbtt_aligned_quad getBakedQuad(float* x, float* y, int code, float scale) {

				stbtt_aligned_quad quad;

				int index = code - 32;
				float iw = 1.0f / width;
				float ih = 1.0f / height;

				const stbtt_bakedchar* info = cdata + index;
				int round_x = (int) floor((*x + info->xoff * scale) + 0.5f);
				int round_y = (int) floor((*y + info->yoff * scale) + 0.5f);

				quad.x0 = round_x;
				quad.y0 = round_y;
				quad.x1 = round_x + (info->x1 - info->x0) * scale;
				quad.y1 = round_y + (info->y1 - info->y0) * scale;

				quad.s0 = info->x0 * iw;
				quad.t0 = info->y0 * ih;
				quad.s1 = info->x1 * iw;
				quad.t1 = info->y1 * ih;

				*x += info->xadvance * scale;

				return quad;
			}

	};

}
