#pragma once

#include "image.hpp"
#include "texture.hpp"

namespace plgl {

	struct Box2D {

		int x, y, w, h;

		Box2D(int x, int y, int w, int h);

		/// Split this box into it's top and bottom parts
		Box2D split_top_bottom(int offset);

		/// Split this box into it's left and right parts
		Box2D split_left_right(int offset);

		/// Check if no pixels are bound by this box
		bool empty() const;

		/// Allocate area from pool
		static Box2D allocate(std::list<Box2D>& boxes, int w, int h);

	};

	struct Sprite {

		Texture* texture;
		int x, y, w, h;

		Sprite() = default;
		Sprite(Texture* texture, int x, int y, int w, int h);

	};

	class Atlas : public Texture {

		private:

			Image atlas;
			std::list<Box2D> pool;

			Sprite packSprite(Image& image, const std::function<void()>& on_resize);

		public:

			Atlas();
			void close();

		public:

			Image& getImage();
			void upload();
			void save(const std::string& path) const final;
			Sprite submit(const std::string& path, const std::function<void()>& on_resize = {});
			Sprite submit(Image& image, const std::function<void()>& on_resize = {});

	};

}
