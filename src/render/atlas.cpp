
#include "atlas.hpp"

namespace plgl {

	/*
	 * Box2D
	 */

	Box2D::Box2D(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	Box2D Box2D::split_top_bottom(int height) {
		int yn = this->y;

		this->y += height;
		this->h -= height;

		return {x, yn, w, height};
	}

	Box2D Box2D::split_left_right(int width) {
		int xn = this->x;

		this->x += width;
		this->w -= width;

		return {xn, y, width, h};
	}

	/// Check if no pixels are bound by this box
	bool Box2D::empty() const {
		return (w == 0) || (h == 0);
	}

	/// allocate the given area from the box set
	Box2D Box2D::allocate(std::list<Box2D>& boxes, int w, int h) {

		for (auto it = boxes.begin(); it != boxes.end(); ++it) {

			Box2D& box = *it;

			// remove degenerate boxes
			if (box.empty()) {
				it = boxes.erase(it);
				continue;
			}

			// our box needs to fit first
			if (box.w < w || box.h < h) {
				continue;
			}

			// a happy little accident!
			if (box.w == w && box.h == h) {
				Box2D match = box;
				boxes.erase(it);
				return match;
			}

			//  width
			if (box.w == w) {
				return box.split_top_bottom(h);
			}

			// height
			if (box.h == h) {
				return box.split_left_right(w);
			}

			Box2D left = box.split_left_right(w);
			Box2D bottom = left.split_top_bottom(h);

			boxes.push_front(left);
			return bottom;

		}

		return {0, 0, 0, 0};

	}

	/*
	 * Sprite
	 */

	Sprite::Sprite(Texture* texture, int x, int y, int w, int h)
	: texture(texture), x(x), y(y), w(w), h(h) {}

	/*
	 * Atlas
	 */

	Sprite Atlas::packSprite(Image& image, const std::function<void()>& on_resize) {

		Box2D box = Box2D::allocate(this->pool, image.width(), image.height());

//		for (Box2D box : pool) {
//			atlas.fill(box.x, box.y, box.w, box.h, {0, 0, 0, 255});
//			atlas.fill(box.x + 3, box.y + 3, box.w - 6, box.h - 6, {255, 255, 255, 255});
//		}

		if (!box.empty()) {
			atlas.blit(box.x, box.y, image);
			return {this, box.x, box.y, box.w, box.h};
		}

		// notify caller
		if (on_resize) {
			on_resize();
		}

		int w = atlas.width();
		int h = atlas.width();

		// retry with bigger atlas
		pool.emplace_back(w, 0, w, h);
		pool.emplace_back(0, h, 2 * w, h);
		atlas.resize(w + w, h + h);

		return packSprite(image, {});
	}

	Atlas::Atlas() {
		atlas = Image::allocate(512, 512);
		pool.emplace_back(0, 0, 512, 512);
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