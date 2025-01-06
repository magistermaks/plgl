
#include "texture.hpp"
#include "internal.hpp"

namespace plgl {

	GLenum Texture::getFormat(int channels) {
		switch (channels) {
			case 4:
				return GL_RGBA;
			case 3:
				return GL_RGB;
			case 1:
				return GL_ALPHA;
		}

		impl::fatal("Unsuported texture channel count: %s!", channels);
	}

	void Texture::upload(const void* data, size_t width, size_t height, size_t channels) {
		glBindTexture(GL_TEXTURE_2D, tid);

		// upload texture data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, getFormat(channels), GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		this->channels = channels;
		this->width = width;
		this->height = height;
	}

	Texture::Texture() {
		glGenTextures(1, &tid);
		glBindTexture(GL_TEXTURE_2D, tid);

		// set texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	Texture::Texture(const char* path)
	: Texture() {
		Image image = Image::loadFromFile(path);
		upload(image);
		image.close();
	}

	void Texture::close() {
		glDeleteTextures(1, &tid);
	}

	void Texture::use() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tid);
	}

	void Texture::upload(Image& image) {
		upload(image.data(), image.width(), image.height(), image.channels());
	}

	GLuint Texture::getTid() const {
		return tid;
	}

	int Texture::getWidth() const {
		return width;
	}

	int Texture::getHeight() const {
		return height;
	}

	Image Texture::getPixels() const {
		Image image = Image::allocate(width, height, 4);
		glBindTexture(GL_TEXTURE_2D, tid);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
		return image;
	}

	void Texture::save(const std::string& path) const {
		Image image = getPixels();
		image.save(path);
		image.close();
	}

}