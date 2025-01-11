
#include "image.hpp"
#include "util.hpp"

namespace plgl {

	/*
	 * Pixel
	 */

	Pixel::Pixel(uint8_t* pixel, uint8_t count)
	: pixel(pixel), count(count) {}

	uint8_t& Pixel::operator [] (uint8_t offset) {
		return pixel[offset];
	}

	const uint8_t& Pixel::operator [] (uint8_t offset) const {
		return pixel[offset];
	}

	void Pixel::set(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		pixel[0] = r;
		pixel[1] = g;
		pixel[2] = b;
		pixel[3] = a;
	}

	void Pixel::set(uint8_t r, uint8_t g, uint8_t b) {
		pixel[0] = r;
		pixel[1] = g;
		pixel[2] = b;
	}

	uint8_t Pixel::size() const {
		return count;
	}

	const uint8_t* Pixel::data() const {
		return pixel;
	}

	uint8_t* Pixel::data() {
		return pixel;
	}

	/*
	 * Image
	 */

	Image::Image() {}

	Image::Image(Type type, void* pixels, int w, int h, int c)
	: type(type), pixels(pixels), w(w), h(h), c(c) {}

	void Image::close() {
		if (pixels != nullptr) {
			if (type == STB_IMAGE) stbi_image_free(pixels);
			if (type == MALLOCED) free(pixels);
			pixels = nullptr;
		}
	}

	void Image::resize(int w, int h) {
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

	void Image::blit(int ox, int oy, Image& image) {
		if (ox < 0 || oy < 0) {
			fault("Can't blit an image at negative offset ({}, {})!", ox, oy);
		}

		if (ox + image.width() > w || oy + image.height() > h) {
			fault("Can't blit an image of size ({}, {}) at offset ({}, {}) as it would fall outside target image bounds ({}, {})!", ox, oy, image.width(), image.height(), w, h);
		}

		if (image.channels() != channels()) {
			fault("Can't blit an image with {} channels into an image with {} channels!", image.channels(), channels());
		}

		for (int y = 0; y < (int) image.height(); y ++) {
			memcpy(pixel(ox, oy + y).data(), image.pixel(0, y).data(), image.width() * image.channels());
		}
	}

	void Image::clear(std::initializer_list<uint8_t> value) {
		if ((int) value.size() != channels()) {
			fault("Can't clear image with {} channels using value with {} channels!", channels(), value.size());
		}

		for (int y = 0; y < (int) h; y ++) {
			for (int x = 0; x < (int) w; x++) {
				memcpy(pixel(x, y).data(), value.begin(), c);
			}
		}
	}

	Pixel Image::pixel(int x, int y) {
		return {((uint8_t*) pixels) + (x + y * w) * channels(), (uint8_t) c};
	}

	void Image::save(const std::string& path) const {
		stbi_write_png(path.c_str(), w, h, c, pixels, w * c);
	}

	Image Image::load(const std::string& path, int channels) {
		int ignored, w, h;
		void* pixels = stbi_load(path.c_str(), &w, &h, &ignored, channels);

		if (!pixels) {
			fault("Unable to load texture: '{}'!", path);
		}

		return {Type::STB_IMAGE, pixels, w, h, channels};
	}

	Image Image::allocate(int w, int h, int channels) {
		return {Type::MALLOCED, malloc(w * h * channels), w, h, channels};
	}

	size_t Image::size() const {
		return width() * height() * channels();
	}

	size_t Image::width() const {
		return w;
	}

	size_t Image::height() const {
		return h;
	}

	size_t Image::channels() const {
		return c;
	}

	const void* Image::data() const {
		return pixels;
	}

	void* Image::data() {
		return pixels;
	}

}