#pragma once

#include "external.hpp"
#include "internal.hpp"

namespace plgl {

	class Pixel {

		private:

			uint8_t* pixel;
			uint8_t count;

		public:

			Pixel(uint8_t* pixel, uint8_t count);

			uint8_t& operator [] (uint8_t offset);
			const uint8_t& operator [] (uint8_t offset) const;

			void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
			void set(uint8_t r, uint8_t g, uint8_t b);

			uint8_t size() const;
			const uint8_t* data() const;
			uint8_t* data();

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

			Image();
			Image(Type type, void* pixels, int w, int h, int c);

			void close();

			void resize(int w, int h);
			void blit(int ox, int oy, Image& image);
			Pixel pixel(int x, int y);
			void save(const std::string& path);

			/**
			 * Returns an image data buffer for the image pointer to by the
			 * given file path and of the given number of channels
			 */
			static Image loadFromFile(const std::string& path, int channels = 4);

			/**
			 * Creates a new uninitialized image of the given
			 * dimensions and channel count.
			 */
			static Image allocate(int w, int h, int channels = 4);

		public:

			size_t size() const;
			size_t width() const;
			size_t height() const;
			size_t channels() const;
			const void* data() const;
			void* data();

	};

}
