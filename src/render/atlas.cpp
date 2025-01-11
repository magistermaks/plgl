
#include "atlas.hpp"

namespace plgl {

	/*
	 * Sprite
	 */

	Sprite::Sprite(Texture* texture, int x, int y, int w, int h)
	: texture(texture), x(x), y(y), w(w), h(h) {}

	bool Sprite::intersects(int x1, int y1, int w, int h) const {
		if (x >= x1 + w || x1 >= x + this->w) return false;
		if (y >= y1 + h || y1 >= y + this->h) return false;

		return true;
	}

	/*
	 * Atlas
	 */

	bool Atlas::canPlaceAt(int x, int y, int w, int h) const {
		if (x + w > (int) atlas.width()) {
			return false;
		}

		if (y + h > (int) atlas.height()) {
			return false;
		}

		for (const auto& value : sprites) {
			if (value.intersects(x, y, w, h)) {
				return false;
			}
		}

		return true;
	}

	Sprite Atlas::packSprite(Image& image, const std::function<void()>& on_resize) {
		for (int x = 0; x < (int) atlas.width(); x += horizontal) {
			for (int y = 0; y < (int) atlas.height(); y += vertical) {
				const int w = image.width();
				const int h = image.height();

				if (canPlaceAt(x, y, w, h)) {
					atlas.blit(x, y, image);

					Sprite sprite {this, x, y, w, h};
					sprites.push_back(sprite);
					return sprite;
				}
			}
		}

		// notify caller
		if (on_resize) {
			on_resize();
		}

		// retry with bigger atlas
		atlas.resize(atlas.width() * 2, atlas.height() * 2);
		return packSprite(image, {});
	}

	Atlas::Atlas() {
		atlas = Image::allocate(512, 512);
	}

	Atlas::Atlas(int vertical, int horizontal) : Atlas() {
		this->vertical = vertical;
		this->horizontal = horizontal;
	}

	void Atlas::close() {
		atlas.close();
		Texture::close();
	}

	Image& Atlas::getImage() {
		return atlas;
	}

	void Atlas::upload() {
		Texture::upload(atlas);
	}

	void Atlas::save(const std::string& path) const {
		atlas.save(path);
	}

	Sprite Atlas::submit(const std::string& path, const std::function<void()>& on_resize) {
		Image image = Image::load(path);
		Sprite sprite = submit(image, on_resize);
		image.close();

		return sprite;
	}

	Sprite Atlas::submit(Image& image, const std::function<void()>& on_resize) {
		return packSprite(image, on_resize);
	}

}