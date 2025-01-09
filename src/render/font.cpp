
#include "font.hpp"
#include "atlas.hpp"

#include "msdfgen.h"
#include "msdfgen-ext.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace plgl {

	static msdfgen::FreetypeHandle* freetype = msdfgen::initializeFreetype();

	/*
	 * Font
	 */

	static FT_Face getFreeType(msdfgen::FontHandle* font) {
		// get around the fact there is no official way to access the underlying freetype font object
		return *reinterpret_cast<FT_Face*>(font);
	}

	bool Font::loadUnicode(msdfgen::FontHandle* font, uint32_t unicode, float scale, float range, const std::function<void()>& on_resize) {
		msdfgen::Shape shape;

		Image image = Image::allocate(resolution, resolution, 4);
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

			Sprite sprite = atlas.submit(image, on_resize);
			image.close();

			info.x0 = sprite.x;
			info.y0 = sprite.y + sprite.h;
			info.x1 = sprite.x + sprite.w;
			info.y1 = sprite.y;

			cdata[unicode] = info;

			// update texture
			atlas.upload();

			return true;
		}

		return false;
	}

	Font::Font(const char* path, int weight)
	: atlas(resolution, resolution) {

		this->base = 100;

		if (!freetype) {
			throw std::runtime_error {"Failed to initialize FreeType library!"};
		}

		this->font = loadFont(freetype, path);

		if (!font) {
			throw std::runtime_error {std::string ("Failed to open font: '") + path + "'"};
		}

		std::vector<msdfgen::FontVariationAxis> axes;
		msdfgen::listFontVariationAxes(axes, freetype, font);

		FT_Face face = getFreeType(this->font);
		printf("Loaded font '%s'", face->family_name);

		for (auto axis : axes) {
			printf(" [Axis: '%s']", axis.name);
		}

		printf("\n");
		msdfgen::setFontVariationAxis(freetype, font, "Weight", weight);

	}

//	void Font::close() {
//		destroyFont(handle);
//		atlas.close();
//	}

	float Font::getScaleForSize(float size) const {
		return size / base;
	}

	GlyphQuad Font::getBakedQuad(float* x, float* y, float scale, int unicode, int prev, const std::function<void()>& on_resize) {

		GlyphQuad quad;

		double kerning = 0;
		float iw = 1.0f / width();
		float ih = 1.0f / height();

		if (prev != 0) {
			msdfgen::getKerning(kerning, font, prev, unicode, msdfgen::FONT_SCALING_EM_NORMALIZED);
		}

		if (kerning != 0) {
			kerning = kerning;
		}

		auto pair = cdata.find(unicode);

		if (pair == cdata.end()) {
			loadUnicode(font, unicode, 64, 6, on_resize);
			return getBakedQuad(x, y, scale, unicode, prev, on_resize);
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

	void Font::use() const {
		atlas.use();
	}

	int Font::handle() const {
		return atlas.handle();
	}

	int Font::width() const {
		return atlas.width();
	}

	int Font::height() const {
		return atlas.height();
	}

}