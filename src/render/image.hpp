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

			/// Write a specific color into the pixel
			void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

			/// Write a specific color into the pixel
			void set(uint8_t r, uint8_t g, uint8_t b);

			/// Get pixel size in bytes
			uint8_t size() const;

			/// Returns a pointer to the start of pixel's data
			const uint8_t* data() const;

			/// Returns a pointer to the start of pixel's data
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

			/**
			 * @brief Resize image
			 *
			 * Internally reallocates the image, previous data is copied
			 * into the new memory and freed (pointers to old data will become invalid)
			 *
			 * @param[in] w Width of the target image in pixels
			 * @param[in] h Height of the target image in pixels
			 */
			void resize(int w, int h);

			/**
			 * @brief Paste image
			 *
			 * Blit (paste) another, smaller, image into this one
			 * at the given position, both images MUST share channel counts
			 *
			 * @param ox    Offset, in pixels, from the left side of this image
			 * @param oy    Offset, in pixels, from the bottom of this image
			 * @param image The image that should be pasted
			 */
			void blit(int ox, int oy, Image& image);

			/**
			 * @brief Clear image
			 *
			 * Clears the image, setting all pixels in the image to the
			 * given value, must contains EXACTLY 'channels()' components
			 *
			 * @example
			 * @code{.cpp}
			 * int main() {
			 *
			 *    // Create 100x100 image
			 *    Image foo = Image::allocate(100, 100);
			 *
			 *    // Fill it with Red=255, Gree=100, Blue=100, Alpha=255 (Opaque)
			 *    foo.clear({255, 100, 100, 255});
			 *
			 *    // Saved image should be of a uniform, reddish, color
			 *    foo.save("my_image.png");
			 *
			 * }
			 * @endcode
			 *
			 * @param value Fill color value list
			 */
			void clear(std::initializer_list<uint8_t> value);


			void fill(int x, int y, int w, int h, std::initializer_list<uint8_t> value);

			Pixel pixel(int x, int y);

			/**
			 * @brief Save image to file.
			 *
			 * Image will be same with equal number of color
			 * channels as this image object.
			 *
			 * @param[in] path Path and filename of the file to save as
			 */
			void save(const std::string& path) const;

			/**
			 * @brief Load image from file.
			 *
			 * Returns an image data buffer for the image pointer to by the
			 * given file path and of the given number of channels.
			 *
			 * @note Number of channels can be different that the
			 *       real channel count of the file.
			 *
			 * @param[in] path     Path to the image file
			 * @param[in] channels Optionally, number of channels to use
			 */
			static Image load(const std::string& path, int channels = 4);

			/**
			 * @brief Allocate new empty image.
			 *
			 * Creates a new uninitialized image of the given
			 * dimensions and channel count. After creation the image
			 * can contain random data, use clear() to fill it with one
			 * specific color/value.
			 *
			 * @see ImageData::clear()
			 *
			 * @param[in] w        Width of the image in pixels
			 * @param[in] h        Height of the image in pixels
			 * @param[in] channels Optionally, number of channels to use
			 */
			static Image allocate(int w, int h, int channels = 4);

		public:

			/// returns the size, in bytes, of the data buffer used by this image
			size_t size() const;

			/// returns the width, in pixels, of the image
			size_t width() const;

			/// returns the height, in pixels, of the image
			size_t height() const;

			/// returns the number of channels (bytes) per pixel
			size_t channels() const;

			/// returns a pointer to the start of images' data buffer
			const void* data() const;

			/// returns a pointer to the start of images' data buffer
			void* data();

	};

}
