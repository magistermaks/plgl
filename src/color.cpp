
#include "color.hpp"

namespace plgl {

	static inline float hue_to_rgb(float p, float q, float t) {
		if (t < 0) t += 1;
		if (t > 1) t -= 1;
		if (t < 1.0f/6.0f) return p + (q - p) * 6 * t;
		if (t < 1.0f/2.0f) return q;
		if (t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6;

		return p;
	}

	/*
	 * RGBA
	 */

	RGBA::RGBA()
	: r(0), g(0), b(0), a(0) {}

	RGBA::RGBA(float r, float g, float b, float a)
	: r(r), g(g), b(b), a(a) {}

	RGBA::RGBA(int rgb, float a)
	: r(unpack_red(rgb)), g(unpack_green(rgb)), b(unpack_blue(rgb)), a(a) {}

	RGBA RGBA::as_rgba() const {
		return *this;
	}

	// https://stackoverflow.com/a/9493060
	HSLA RGBA::hsla() const {
		float r = impl::normalize(this->r);
		float g = impl::normalize(this->g);
		float b = impl::normalize(this->b);

		float vmax = std::max({r, g, b});
		float vmin = std::min({r, g, b});

		float h, s, l = (vmax + vmin) / 2;

		// achromatic
		if (vmax == vmin) {
			h = s = 0;
		} else {
			float d = vmax - vmin;
			s = l > 0.5 ? d / (2 - vmax - vmin) : d / (vmax + vmin);

			if (vmax == r) h = (g - b) / d + (g < b ? 6 : 0);
			if (vmax == g) h = (b - r) / d + 2;
			if (vmax == b) h = (r - g) / d + 4;
		}

		return {(h / 6) * TAU, s, l, this->a};
	}

	// get red channel value [0, 255]
	float RGBA::red() const {
		return r;
	}

	// get green channel value [0, 255]
	float RGBA::green() const {
		return g;
	}

	// get blue channel value [0, 255]
	float RGBA::blue() const {
		return b;
	}

	float RGBA::alpha() const {
		return a;
	}

	bool RGBA::achromatic() const {
		return r == g && g == b;
	}

	int RGBA::pack_rgb() const {
		return impl::denormalize(b) | (impl::denormalize(g) << 8) | (impl::denormalize(r) << 16);
	}

	int RGBA::pack_argb() const {
		return pack_rgb() + (impl::denormalize(a) << 24);
	}

	std::string RGBA::str() const {
		return format("rgba({}, {}, {}, {})", impl::denormalize(r), impl::denormalize(g), impl::denormalize(b), impl::denormalize(a));
	}

	/*
	 * HSLA
	 */

	RGBA HSLA::as_rgba() const {
		return rgba();
	}

	HSLA::HSLA()
	: h(0), s(0), l(0), a(0) {}

	HSLA::HSLA(float h, float s, float l, float a)
	: h(h), s(s), l(l), a(a) {}

	// https://stackoverflow.com/a/9493060
	RGBA HSLA::rgba() const {
		float r, g, b;
		float sh = h / TAU;

		if (achromatic()) {
			r = g = b = l;
		} else {
			float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
			float p = 2 * l - q;

			r = hue_to_rgb(p, q, sh + 0.33333333f);
			g = hue_to_rgb(p, q, sh);
			b = hue_to_rgb(p, q, sh - 0.33333333f);
		}

		return {r * 255, g * 255, b * 255, this->a};
	}

	// get the hue in radians [0, TAU]
	float HSLA::hue() const {
		return h;
	}

	// get the color saturation [0, 1]
	float HSLA::sat() const {
		return s;
	}

	// get the color luminance (brightness) [0, 1]
	float HSLA::lum() const {
		return l;
	}

	float HSLA::alpha() const {
		return a;
	}

	bool HSLA::achromatic() const {
		return s == 0;
	}

	std::string HSLA::str() const {
		return format("hsla({}, {}%, {}%, {})", (int) round((h / TAU) * 360), (int) round(s * 100), (int) round(l * 100), impl::denormalize(a));
	}


}
