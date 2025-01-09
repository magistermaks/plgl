#pragma once

#include "image.hpp"
#include "texture.hpp"

namespace plgl {

	struct Sprite {

		Texture* texture;
		int x, y, w, h;

		Sprite() = default;
		Sprite(Texture* texture, int x, int y, int w, int h);

		bool intersects(int x1, int y1, int w, int h) const;

	};

	class Atlas : public Texture {

		private:

			int vertical = 1;
			int horizontal = 1;
			Image atlas;
			std::vector<Sprite> sprites;

			bool canPlaceAt(int x, int y, int w, int h) const;
			Sprite packSprite(Image& image, const std::function<void()>& on_resize);

		public:

			Atlas();
			Atlas(int vertical, int horizontal);

			void close();

		public:

			Image& getImage();
			void upload();
			void save(const std::string& path) const final;
			Sprite submit(const std::string& path, const std::function<void()>& on_resize = {});
			Sprite submit(Image& image, const std::function<void()>& on_resize = {});

	};

}
