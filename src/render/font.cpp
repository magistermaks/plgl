
#include "font.hpp"
#include "atlas.hpp"

#include "msdfgen.h"
#include "msdfgen-ext.h"

namespace plgl {

	static msdfgen::FreetypeHandle* freetype = msdfgen::initializeFreetype();

	bool Font::loadUnicode(msdfgen::FontHandle* font, uint32_t unicode, int size, float scale, float range, bool* flush) {
		msdfgen::Shape shape;

		Image image = Image::allocate(size, size, 4);
		GlyphInfo info;

		if (loadGlyph(shape, font, unicode, msdfgen::FONT_SCALING_EM_NORMALIZED, &info.advance)) {
			shape.normalize();

			info.advance *= scale;
			auto box = shape.getBounds();

			float tx = (1 - (box.r - box.l)) * 0.5f - box.l;
			float ty = (1 - (box.t - box.b)) * 0.5f - box.b;

			info.xoff = (-tx) * scale;
			info.yoff = ty * scale;

			edgeColoringSimple(shape, 3.0);

			msdfgen::Bitmap<float, 3> msdf(image.width(), image.height());

			msdfgen::SDFTransformation transform {
				msdfgen::Projection(scale, msdfgen::Vector2(tx, ty)),
				msdfgen::Range(range) / scale
			};

			msdfgen::generateMSDF(msdf, shape, transform);

			// convert to some normal format...
			for (int x = 0; x < msdf.width(); x++) {
				for (int y = 0; y < msdf.height(); y++) {
					float* pixel = msdf(x, y);

					uint8_t r = msdfgen::pixelFloatToByte(pixel[0]);
					uint8_t g = msdfgen::pixelFloatToByte(pixel[1]);
					uint8_t b = msdfgen::pixelFloatToByte(pixel[2]);
					uint8_t a = msdfgen::pixelFloatToByte(pixel[3]);

					image.pixel(x, y).set(r, g, b, a);
				}
			}

			Sprite sprite = atlas.submitImage(image, flush);
			image.close();

			info.x0 = sprite.x;
			info.y0 = sprite.y + sprite.h;
			info.x1 = sprite.x + sprite.w;
			info.y1 = sprite.y;

			cdata[unicode] = info;
			modified = true;

			return true;
		}

		return false;
	}

	/*
	 * Font
	 */

	Font::Font(const char* path, float height) : Texture() {

		this->base = 100;

		if (!freetype) {
			throw std::runtime_error {"Failed to start Free Type library!"};
		}

		this->handle = loadFont(freetype, path);

		if (!handle) {
			throw std::runtime_error {std::string ("Failed to open font: '") + path + "'"};
		}

		std::vector<msdfgen::FontVariationAxis> axes;
		msdfgen::listFontVariationAxes(axes, freetype, handle);

		for (auto axis : axes) {
			printf(" * Axis: %s\n", axis.name);
		}

		msdfgen::setFontVariationAxis(freetype, handle, "Weight", 400);

	}

//	void Font::close() {
//		destroyFont(handle);
//		atlas.close();
//	}

	float Font::getScaleForSize(float size) const {
		return size / base;
	}

	float Font::getFontLineGap() const {

	}

	stbtt_aligned_quad Font::getBakedQuad(bool* flush, float* x, float* y, int unicode, float scale, int prev) {

		stbtt_aligned_quad quad;

		double kerning = 0;
		float iw = 1.0f / width;
		float ih = 1.0f / height;

		if (prev != 0) {
			msdfgen::getKerning(kerning, handle, prev, unicode, msdfgen::FONT_SCALING_EM_NORMALIZED);
		}

		if (kerning != 0) {
			kerning = kerning;
		}

		auto pair = cdata.find(unicode);

		if (pair == cdata.end()) {
			loadUnicode(handle, unicode, 64, 64, 6, flush);

			prepare();
			return getBakedQuad(flush, x, y, unicode, scale, prev);
		}

		GlyphInfo& info = pair->second;

		int round_x = (int) floor((*x + info.xoff * scale) + 0.5f);
		int round_y = (int) floor((*y + info.yoff * scale) + 0.5f);

		quad.x0 = round_x + kerning * base * scale;
		quad.y0 = round_y;
		quad.x1 = round_x + (info.x1 - info.x0) * scale;
		quad.y1 = round_y + (info.y1 - info.y0) * scale;

		quad.s0 = info.x0 * iw;
		quad.t0 = info.y1 * ih;
		quad.s1 = info.x1 * iw;
		quad.t1 = info.y0 * ih;

		*x += info.advance * scale;
		return quad;

	}

	void Font::prepare() {
		if (modified) {
			Image& image = atlas.getImage();
			upload(image.data(), image.width(), image.height(), 4);
		}
	}

}