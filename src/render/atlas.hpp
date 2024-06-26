#pragma once

#include "image.hpp"
#include "texture.hpp"

namespace plgl {

	struct Sprite {

		Texture* texture;
		int x, y, w, h;

		Sprite() {}

		Sprite(Texture* texture, int x, int y, int w, int h)
		: texture(texture), x(x), y(y), w(w), h(h) {}

		bool intersects(int x1, int y1, int w, int h) const {
			if (x >= x1 + w || x1 >= x + this->w) return false;
			if (y >= y1 + h || y1 >= y + this->h) return false;

			return true;
		}

	};

	class Atlas {

		private:

			bool frozen;
			Texture texture;
			Image atlas;
			std::vector<Sprite> sprites;

			bool canPlaceAt(int x, int y, int w, int h) {
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

			Sprite packSprite(Image& image) {
				for (int x = 0; x < (int) atlas.width(); x ++) {
					for (int y = 0; y < (int) atlas.height(); y ++) {
						const int w = image.width();
						const int h = image.height();

						if (canPlaceAt(x, y, w, h)) {
							atlas.blit(x, y, image);

							Sprite sprite {&texture, x, y, w, h};
							sprites.push_back(sprite);
							return sprite;
						}
					}
				}

				// retry with bigger atlas
				atlas.resize(atlas.width() * 2, atlas.height() * 2);
				return packSprite(image);
			}

		public:

			Atlas() {
				atlas = Image::allocate(512, 512);
				frozen = false;
			}

			void close() {
				atlas.close();
				texture.close();
			}

			Image& getImage() {
				return atlas;
			}

			Texture& getTexture() {
				return texture;
			}

			void upload() {
				if (frozen) {
					impl::fatal("Can't upload frozen atlas!");
				}

				texture.upload(atlas);
			}

			void freeze() {
				if (frozen) {
					impl::fatal("Can't freeze frozen atlas!");
				}

				frozen = true;
				atlas.close();
			}

			void save(const std::string& path) {
				if (frozen) {
					impl::fatal("Can't save frozen atlas!");
				}

				atlas.save(path);
			}

			Sprite submitFile(const std::string& path) {
				if (frozen) {
					impl::fatal("Can't modify frozen atlas!");
				}

				Image image = Image::loadFromFile(path);
				Sprite sprite = submitImage(image);
				image.close();

				return sprite;
			}

			Sprite submitImage(Image& image) {
				if (frozen) {
					impl::fatal("Can't modify frozen atlas!");
				}

				return packSprite(image);
			}

	};

}
