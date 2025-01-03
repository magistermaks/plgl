#pragma once

#include "external.hpp"
#include "internal.hpp"
#include "math.hpp"

namespace plgl {

	class RGBA;
	class HSLA;

	struct Color {

		virtual ~Color() = default;
		virtual RGBA as_rgba() const = 0;

	};

	class RGBA : public Color {

		private:

			mutable float r, g, b, a;

			RGBA as_rgba() const override;

		public:

			RGBA();
			RGBA(float r, float g, float b, float a = 255.0f);
			RGBA(int rgb, float a = 255.0f);

			HSLA hsla() const;

			/// get red channel value [0, 255]
			float red() const;

			/// get green channel value [0, 255]
			float green() const;

			/// get blue channel value [0, 255]
			float blue() const;

			/// get alpha channel value [0, 255]
			float alpha() const;

			/// check if this color is achromatic
			bool achromatic() const;

			/// pack into RGB integer
			int pack_rgb() const;

			/// pack into ARGB integer
			int pack_argb() const;

			/// Convert to a human-readable string
			std::string str() const;

	};

	class HSLA : public Color {

		private:

			mutable float h, s, l, a;

			RGBA as_rgba() const override;

		public:

			HSLA();
			HSLA(float h, float s, float l, float a = 255.0f);

			RGBA rgba() const;

			/// get the hue in radians [0, TAU]
			float hue() const;

			/// get the color saturation [0, 1]
			float sat() const;

			/// get the color luminance (brightness) [0, 1]
			float lum() const;

			/// get alpha channel value [0, 255]
			float alpha() const;

			/// check if this color is achromatic
			bool achromatic() const;

			/// Convert to a human-readable string
			std::string str() const;

	};

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

	inline RGBA unpack_argb(int argb) {
		return {argb, (float) unpack_alpha(argb)};
	}

}

