#pragma once

#include "external.hpp"
#include "internal.hpp"

namespace plgl {

	class Pixel {

		private:

			uint8_t* pixel;
			uint8_t count;

		public:

			Pixel(uint8_t* pixel, uint8_t count)
			: pixel(pixel), count(count) {}

			inline uint8_t& operator [] (uint8_t offset) {
				return pixel[offset];
			}

			inline const uint8_t& operator [] (uint8_t offset) const {
				return pixel[offset];
			}

			inline void rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
				pixel[0] = r;
				pixel[1] = g;
				pixel[2] = b;
				pixel[3] = a;
			}

			inline void rgb(uint8_t r, uint8_t g, uint8_t b) {
				pixel[0] = r;
				pixel[1] = g;
				pixel[2] = b;
			}

			inline uint8_t size() const {
				return count;
			}

			inline const uint8_t* data() const {
				return pixel;
			}

			inline uint8_t* data() {
				return pixel;
			}

	};

	class Image {

		private:

			enum Type {
				STB_IMAGE,
				MALLOCED,
				UNBACKED
			};

			Type type;
			void* pixels;
			size_t w, h, c;

		public:

			Image() {}

			Image(Type type, void* pixels, int w, int h, int c)
			: type(type), pixels(pixels), w(w), h(h), c(c) {}

			void close() {
				if (pixels != nullptr) {
					if (type == STB_IMAGE) stbi_image_free(pixels);
					if (type == MALLOCED) free(pixels);
					pixels = nullptr;
				}
			}

			void resize(int w, int h) {
				if (w * h * channels() < size()) {
					return;
				}

				Image buffer = Image::allocate(w, h, channels());
				buffer.blit(0, 0, *this);

				// no matter how our image was allocated, free it now
				close();

				this->pixels = buffer.pixels;
				this->w = w;
				this->h = h;
			}

			void blit(int ox, int oy, Image& image) {
				if (ox + image.width() > w || oy + image.height() > h) {
					impl::fatal("Can't blit-in the given image, invalid placement!");
				}

				if (image.channels() != channels()) {
					impl::fatal("Can't blit-in the given image, invalid channel count!");
				}

				for (int y = 0; y < (int) image.height(); y ++) {
					memcpy(pixel(ox, oy + y).data(), image.pixel(0, y).data(), image.width() * image.channels());
				}
			}

			Pixel pixel(int x, int y) {
				return {((uint8_t*) pixels) + (x + y * w) * channels(), (uint8_t) c};
			}

			void save(const std::string& path) {
				stbi_write_png(path.c_str(), w, h, c, pixels, w * c);
			}

			/**
			 * Returns an image data buffer for the image pointer to by the
			 * given file path and of the given number of channels
			 */
			static Image loadFromFile(const std::string& path, int channels = 4) {
				int ignored, w, h;
				void* pixels = stbi_load(path.c_str(), &w, &h, &ignored, channels);

				if (!pixels) {
					impl::fatal("Unable to load texture: '%s'!", path);
				}

				return {Type::STB_IMAGE, pixels, w, h, channels};
			}

			/**
			 * Creates a new uninitialized image of the given
			 * dimensions and channel count.
			 */
			static Image allocate(int w, int h, int channels = 4) {
				return {Type::MALLOCED, malloc(w * h * channels), w, h, channels};
			}

		public:

			inline size_t size() const {
				return width() * height() * channels();
			}

			inline size_t width() const {
				return w;
			}

			inline size_t height() const {
				return h;
			}

			inline size_t channels() const {
				return c;
			}

			inline const void* data() const {
				return pixels;
			}

			inline void* data() {
				return pixels;
			}

	};

}
