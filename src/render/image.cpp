
#include "image.hpp"

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

	Pixel Image::pixel(int x, int y) {
		return {((uint8_t*) pixels) + (x + y * w) * channels(), (uint8_t) c};
	}

	void Image::save(const std::string& path) {
		stbi_write_png(path.c_str(), w, h, c, pixels, w * c);
	}

	Image Image::loadFromFile(const std::string& path, int channels) {
		int ignored, w, h;
		void* pixels = stbi_load(path.c_str(), &w, &h, &ignored, channels);

		if (!pixels) {
			impl::fatal("Unable to load texture: '%s'!", path);
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