#pragma once

#include "external.hpp"
#include "internal.hpp"
#include "math.hpp"

namespace plgl {

	class RGBA;
	class HSLA;

	namespace impl {

		inline float hue_to_rgb(float p, float q, float t) {
			if (t < 0) t += 1;
			if (t > 1) t -= 1;
			if (t < 1.0f/6.0f) return p + (q - p) * 6 * t;
			if (t < 1.0f/2.0f) return q;
			if (t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6;

			return p;
		}

		struct Color {

			virtual ~Color() = default;
			virtual RGBA as_rgba() const = 0;

		};

	}

	inline int unpack_alpha(int argb) {
		return (argb >> 24) & 0xFF;
	}

	inline int unpack_red(int argb) {
		return (argb >> 16) & 0xFF;
	}

	inline int unpack_green(int argb) {
		return (argb >> 8) & 0xFF;
	}

	inline int unpack_blue(int argb) {
		return argb & 0xFF;
	}

	class RGBA : public impl::Color {

		private:

			mutable float r, g, b, a;

			RGBA as_rgba() const override {
				return *this;
			}

		public:

			RGBA()
			: r(0), g(0), b(0), a(0) {}

			RGBA(float r, float g, float b, float a = 255.0f)
			: r(r), g(g), b(b), a(a) {}

			RGBA(int rgb, float a = 255.0f)
			: r(unpack_red(rgb)), g(unpack_green(rgb)), b(unpack_blue(rgb)), a(a) {}

			HSLA hsla() const;

			// get red channel value [0, 255]
			float red() const {
				return r;
			}

			// get green channel value [0, 255]
			float green() const {
				return g;
			}

			// get blue channel value [0, 255]
			float blue() const {
				return b;
			}

			float alpha() const {
				return a;
			}

			bool achromatic() const {
				return r == g && g == b;
			}

			int pack_rgb() const {
				return impl::renorm(b) | (impl::renorm(g) << 8) | (impl::renorm(r) << 16);
			}

			int pack_argb() const {
				return pack_rgb() + (impl::renorm(a) << 24);
			}

			std::string str() const {
				return format("rgba(%s, %s, %s, %s)", impl::renorm(r), impl::renorm(g), impl::renorm(b), impl::renorm(a));
			}

	};

	class HSLA : public impl::Color {

		private:

			mutable float h, s, l, a;

			RGBA as_rgba() const override {
				return rgba();
			}

		public:

			HSLA()
			: h(0), s(0), l(0), a(0) {}

			HSLA(float h, float s, float l, float a = 255.0f)
			: h(h), s(s), l(l), a(a) {}

			RGBA rgba() const;

			// get the hue in radians [0, TAU]
			float hue() const {
				return h;
			}

			// get the color saturation [0, 1]
			float sat() const {
				return s;
			}

			// get the color luminance (brightness) [0, 1]
			float lum() const {
				return l;
			}

			float alpha() const {
				return a;
			}

			bool achromatic() const {
				return s == 0;
			}

			std::string str() const {
				return format("hsla(%s, %s%%, %s%%, %s)", (int) round((h / TAU) * 360), (int) round(s * 100), (int) round(l * 100), impl::renorm(a));
			}

	};

	inline RGBA unpack_argb(int argb) {
		return {argb, (float) unpack_alpha(argb)};
	}

}

// https://stackoverflow.com/a/9493060
inline plgl::RGBA plgl::HSLA::rgba() const {
	float r, g, b;
	float sh = h / TAU;

	if (achromatic()) {
		r = g = b = l;
	} else {
		float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
		float p = 2 * l - q;

		r = impl::hue_to_rgb(p, q, sh + 0.33333333f);
		g = impl::hue_to_rgb(p, q, sh);
		b = impl::hue_to_rgb(p, q, sh - 0.33333333f);
	}

	return {r * 255, g * 255, b * 255, this->a};
}

// https://stackoverflow.com/a/9493060
inline plgl::HSLA plgl::RGBA::hsla() const {
	float r = impl::norm(this->r);
	float g = impl::norm(this->g);
	float b = impl::norm(this->b);

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
