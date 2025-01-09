#pragma once

#include "external.hpp"
#include "image.hpp"

namespace plgl {

	class PixelBuffer {

		public:

			virtual ~PixelBuffer();

			virtual void use() const = 0;
			virtual int handle() const = 0;
			virtual int width() const = 0;
			virtual int height() const = 0;

	};

	class Texture : public PixelBuffer {

		protected:

			GLuint tid;
			int c, w, h;

			static GLenum format(int channels);
			void upload(const void* data, int width, int height, int channels);

		public:

			Texture();
			Texture(const char* path);

			/// Free resources associated with this texture
			void close();

			/// Upload an image into this Texture
			void upload(Image& image);

			/// Bind this OpenGL Texture
			void use() const override;

			/// Get OpenGL Texture handle
			int handle() const override;

			/// Get image width in pixels
			int width() const override;

			/// Get image height in pixels
			int height() const override;

			/// Copy image from from Texture into a Image buffer
			Image pixels() const;

			/// Copy image from Texture and save it into a file
			virtual void save(const std::string& path) const;

	};

}
