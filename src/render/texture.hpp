#pragma once

#include "external.hpp"
#include "image.hpp"

namespace plgl {

	class Texture {

		protected:

			GLuint tid;
			int channels;
			int width;
			int height;

			static GLenum getFormat(int channels);
			void upload(const void* data, size_t width, size_t height, size_t channels);

		public:

			Texture();
			Texture(const char* path);

			/// Free resources associated with this texture
			void close();

			/// Bind this OpenGL Texture
			void use() const;

			/// Upload an image into this Texture
			void upload(Image& image);

			/// Get OpenGL Texture handle
			GLuint getTid() const;

			/// Get image width in pixels
			int getWidth() const;

			/// Get image height in pixels
			int getHeight() const;

			/// Copy image from from Texture into a Image buffer
			Image getPixels() const;

			/// Copy image from Texture and save it into a file
			void save(const std::string& path) const;

	};

}
