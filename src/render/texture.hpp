#pragma once

namespace plgl {

	class Texture {

		protected:
		
			GLuint tid;
			int width;
			int height;
		
			GLenum getFormat(int channels) {
				switch (channels) {
					case 4: return GL_RGBA;
					case 3: return GL_RGB;
					case 1: return GL_ALPHA;
				}
	
				impl::fatal("Unsuported texture channel count: %s!", channels);
			}

			void init(unsigned char* data, int width, int height, int channels) {
				glGenTextures(1, &tid);
				glBindTexture(GL_TEXTURE_2D, tid);

				// set texture wrapping
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				// set texture filtering
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// upload texture data
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, getFormat(channels), GL_UNSIGNED_BYTE, data);
		    	glGenerateMipmap(GL_TEXTURE_2D);

				this->width = width;
				this->height = height;
			}

			Texture() {}

		public:

			Texture(unsigned char* data, int width, int height, int channels = 4) {
				init(data, width, height, channels);
			}

			Texture(const char* path) {
				int width, height, channels;
				unsigned char* data = stbi_load(path, &width, &height, &channels, 4);
	
				if (data == nullptr) {
					impl::fatal("Unable to load texture: '%s'!", path);
				}

				init(data, width, height, 4);
				stbi_image_free(data);
			}

			~Texture() {
				glDeleteTextures(1, &tid);
			}
		
			void use() {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tid);
			}

			GLuint getTid() {
				return tid;
			}	

			int getWidth() {
				return width;
			}

			int getHeight() {
				return height;
			}

	};

}
