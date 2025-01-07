
#include "font.hpp"
#include "atlas.hpp"

#include "msdfgen.h"
#include "msdfgen-ext.h"

namespace plgl {

	static bool loadUnicode(msdfgen::FontHandle* font, uint32_t unicode, float scale, float trans, float range, Image& image, GlyphInfo& info) {
		msdfgen::Shape shape;

		if (loadGlyph(shape, font, unicode, msdfgen::FONT_SCALING_EM_NORMALIZED, &info.advance)) {
			shape.normalize();

			info.advance *= scale;

			auto box = shape.getBounds();
			info.xoff = (box.l - trans) * scale;
			info.yoff = (box.b - trans) * scale;

			edgeColoringSimple(shape, 3.0);

			msdfgen::Bitmap<float, 3> msdf(image.width(), image.height());

			msdfgen::SDFTransformation transform {
				msdfgen::Projection(scale, msdfgen::Vector2(trans, trans)),
				msdfgen::Range(range)
			};

			msdfgen::generateMSDF(msdf, shape, transform);

			for (int x = 0; x < msdf.width(); x ++) {
				for (int y = 0; y < msdf.height(); y ++) {
					float* pixel = msdf(x, y);

					uint8_t r = msdfgen::pixelFloatToByte(pixel[0]);
					uint8_t g = msdfgen::pixelFloatToByte(pixel[1]);
					uint8_t b = msdfgen::pixelFloatToByte(pixel[2]);
					uint8_t a = msdfgen::pixelFloatToByte(pixel[3]);

					image.pixel(x, y).set(r, g, b, a);
				}
			}

			return true;
		}

		return false;
	}

	/*
	 * Font
	 */

	Font::Font(const char* path, float height) : Texture() {

		this->base = height;

		Atlas atlas;
		Image image = Image::allocate(128, 128);

		if (msdfgen::FreetypeHandle* freetype = msdfgen::initializeFreetype()) {
			if (msdfgen::FontHandle* font = loadFont(freetype, path)) {

				for (int unicode = ' '; unicode <= '~'; unicode ++) {

					GlyphInfo info;
					if (loadUnicode(font, unicode, 128, 0.125, 0.125, image, info)) {

						Sprite sprite = atlas.submitImage(image);

						info.x0 = sprite.x;
						info.y0 = sprite.y + sprite.h;
						info.x1 = sprite.x + sprite.w;
						info.y1 = sprite.y;

						cdata[unicode - ' '] = info;

					} else {
						throw std::runtime_error {"Failed to load glyph at unicode: " + std::to_string(unicode)};
					}

				}

				destroyFont(font);
			} else {
				throw std::runtime_error {std::string ("Failed to open font: '") + path + "'"};
			}


			deinitializeFreetype(freetype);
		}

		Image& combined = atlas.getImage();
		image.close();

		upload(combined.data(), combined.width(), combined.height(), 4);
		atlas.close();

	}

	float Font::getScaleForSize(float size) const {
		return size / base;
	}

	float Font::getFontLineGap() const {

	}

	stbtt_aligned_quad Font::getBakedQuad(float* x, float* y, int code, float scale) const {

		stbtt_aligned_quad quad;

		int index = code - 32;
		float iw = 1.0f / width;
		float ih = 1.0f / height;

		const GlyphInfo* info = cdata + index;

		int round_x = (int) floor((*x + info->xoff * scale) + 0.5f);
		int round_y = (int) floor((*y + info->yoff * scale) + 0.5f);

		quad.x0 = round_x;
		quad.y0 = round_y;
		quad.x1 = round_x + (info->x1 - info->x0) * scale;
		quad.y1 = round_y + (info->y1 - info->y0) * scale;

		quad.s0 = info->x0 * iw;
		quad.t0 = info->y1 * ih;
		quad.s1 = info->x1 * iw;
		quad.t1 = info->y0 * ih;

		*x += info->advance * scale;
		return quad;

	}

}